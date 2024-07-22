#pragma once
#include "memprog.h"
#include "memprog_config.h"
#include <cstring>
#include <string>

class MemProg {
    private:
        static constexpr uint32_t BufferSize = MEMPROG_BUFFER_SIZE;
        static constexpr uint32_t NumBuffers = MEMPROG_NUM_BUFFERS;
        static constexpr uint32_t NumInterfaces = MEMPROG_NUM_INTERFACES;

        // NOTE: ****These must be defined in a source file in the MCU firmware****
        // Array of base pointers to the MemProg subclass instances
        static MemProg * const Interfaces[NumInterfaces];
        // Address of the parameters structure
        static volatile MEMPROG_PARAM Param;
        // Address of the buffer descriptor tables structure
        static volatile MEMPROG_BDT BufferDescriptors[NumBuffers];
        // Address of the buffers
        static volatile uint8_t Buffers[NumBuffers][BufferSize];
        // Timekeeping function. Return millseconds.
        static uint32_t (* const volatile time_ms)();

        // Optional debug putc function
        static void (* const dputc)(uint8_t c);
        // Optional debug pin write function.
        static void (* const dset)(uint8_t pin, bool state);
        // NOTE: *******************************************************************

        static constexpr uint8_t hex[] = "0123456789ABCDEF";

    protected:
        using CMD_FUNC = void (MemProg::*)();
        MEMPROG_PARAM LocalParam;

    private:
    #ifndef MEMPROG_LOG_LEVEL
    #define MEMPROG_LOG_LEVEL MEMPROG_LOG_OFF
    #endif
        // Log verbosity
        static constexpr uint8_t LogLevel = MEMPROG_LOG_LEVEL;
        // Default number of buffers `GetTXStream` should try to acquire
        static constexpr uint8_t DEFAULT_TX_BUFFER_AFFINITY = 1;

        MEMPROG_HANDLER_CONFIG HandlerConfig;

        CMD_FUNC CurrentHandler;
        bool Active;
        uint8_t Interface;
        uint8_t TXSequence;
        uint8_t ActiveTXIdx;
        uint8_t RXSequence;
        uint8_t ActiveRXIdx;

    public:
        MemProg() = default;
        ~MemProg() = default;

        static void StaticInit() {
            // Clear buffer descriptors
            for (auto & bdt : BufferDescriptors) {
                memset((uint8_t*)&bdt, 0, sizeof(MEMPROG_BDT));
            }

            // Initialize each interface
            for (uint8_t i = 0; i < NumInterfaces; i++) {
                auto& inst = Interfaces[i];
                inst->Interface = i;
                inst->Init();
            }

            Param.Status = _MEMPROG_STATUS_IDLE;
            ReleaseToken();
        }

        static void StaticRun() {
            
        }

    protected:
        virtual void Init() {}

        
        /**
         * @brief This method can be overridden to use a custom handler for a given command code.
         *
         * @param Command Command code received from host
         * @return Pointer to the method to use as a command handler
         *      Return null ptr if the provided command code should use the defult (build-in) handler
         */
        virtual CMD_FUNC GetHandler(MEMPROG_CMD Command) {
            return nullptr;
        }

        // stubs for global commands
        virtual inline void CMD_MASS_ERASE() {DEFAULT_HANDLER();}
        virtual inline void CMD_ERASE_RANGE() {DEFAULT_HANDLER();}
        virtual inline void CMD_PROG_VERIFY() {DEFAULT_HANDLER();}
        virtual inline void CMD_PROG() {DEFAULT_HANDLER();}
        virtual inline void CMD_CRC() {DEFAULT_HANDLER();}
        virtual inline void CMD_READ() {DEFAULT_HANDLER();}

        // The following functions are used in command handlers to send and receive data.
        // Any non-zero return value indicates an error, and these shall be caught be command handlers and cause
        // them to stop execution. In case the command handler is malformed and ignores the return value, these
        // functions also wet `LocalParam.Code` if an error occurs. Whis will cause memprog to abort the 
        // command handler the next time it returns. 


        /**
         * @brief Attempt to allocate a tranmission buffer where data can be written. If no buffer is currently available,
         * Ptr will be set to nullptr
         * 
         * @param Ptr Pointer to the next free byte where data can be written
         * @param Address Target memory address of the next byte which will be written to the stream
         * @param Length Number of consecutive butes that can be written to the pointer
         * @param MinLength Minimum number of consecutive bytes that must be available. If the current buffer does
         * not have enough space, it will be flushed, and a new buffer will be acquired.
         * @return 0 on success
         */
        uint32_t GetTXStream(uint8_t *&Ptr, uint32_t Address, uint32_t &Length, uint32_t MinLength=0) {
            if (TxSequence & 0x80) {
                // Already sent last buffer
                LocalParam.Code = 1;
                LocalParam.Status = _MEMPROG_STATUS_ERR_TX_STREAM;
                return LocalParam.Code;
            }

            if (MinLength > BufferSize) {
                LocalParam.Code = 2;
                LocalParam.Status = _MEMPROG_STATUS_ERR_TX_STREAM;
                return LocalParam.Code;
            }

            uint8_t NeededBuffers = HandlerConfig.TargetTXBufferAffinity;
            // Loop through all the buffers, acquiring free ones as needed
            for (uint8_t i = 0; i < NumBuffers && NeededBuffers; i++) {
                volatile MEMPROG_BDT &bdt = BufferDescriptors[i];
                if (bdt.Token != MEMPROG_TOKEN_TARGET) {
                    continue;
                }
                memory_sync();

                if (bdt.Interface == Interface && bdt.Status == MEMPROG_BUFFER_STATUS_PENDING) {
                    // THis is one of our Tx buffers
                } else if (bdt.Status == MEMPROG_BUFFER_STATUS_FREE) {
                    // This is a free buffer, acquire it
                    bdt.Status = MEMPROG_BUFFER_STATUS_PENDING;
                    bdt.Interface = Interface;
                    bdt.Address = Address;
                    bdt.Length = 0;
                } else {
                    // Not a buffer we can use for transmission
                    continue;
                }
                
                if (ActiveTXIdx == 0xFF) {
                    ActiveTXIdx = i;
                }
                NeededBuffers--;
            }

            // Failed to acquire any buffers
            if (ActiveTXIdx == 0xFF) {
                Ptr = nullptr;
                Length = 0;
                return 0;
            }
            
            volatile MEMPROG_BDT &bdt = BufferDescriptors[ActiveTXIdx];
            bool Flush = false;

            if ((bdt.Length + MinLength) > BufferSize) {
                // If there is not enough space in the current buffer, it must be flushed
                Flush = true;
            } else if (Address != (bdt.Address + bdt.Length)) {
                // If the address is not contiguous, the current buffer must be flushed if it's not empty.
                // Otherwise, just change the buffer address
                if (bdt.Length) {
                    Flush = true;
                } else {
                    bdt.Address = Address;
                }
            }

            if (Flush) {
                uint32_t Error;
                if ((Error = WriteStream(0, false, true)) != 0) {
                    LocalParam.Code = (Error << 4) | 3;
                    LocalParam.Status = _MEMPROG_STATUS_ERR_TX_STREAM;
                    return LocalParam.Code;
                }

                // Call GetTXStream again to switch to the next allocated buffer or try acquiring a new buffer
                return GetTXStream(Ptr, Address, Length, MinLength);
            }

            Ptr = (uint8_t*) &Buffers[ActiveTXIdx][bdt.Length];
            Length = BufferSize - bdt.Length;
            return 0;
        }

        /**
         * Notify MemProg that data has been written to the pointer returned by `GetTXStream`
         *
         * @param Length Number of bytes written 
         * @param Last Indicate to the host that no further data will be transmitted. It is necessary to set this
         * during the last call to `WriteStream`
         * @param Flush Force the current TX buffer to be sent to the host
         * @return 0 on success
         */
        uint32_t WriteStream(uint32_t Length, bool Last=false, bool Flush=false) {
            if (ActiveTXIdx == 0xFF) {
                // No buffer reserved
                LocalParam.Code = 1;
                LocalParam.Status = _MEMPROG_STATUS_ERR_WRITE_STREAM;
                return LocalParam.Code;
            }

            volatile MEMPROG_BDT &bdt = BufferDescriptors[ActiveTXIdx]; 

            bdt.Length += Length;

            if (bdt.Length > BufferSize) {
                LocalParam.Code = 2;
                LocalParam.Status = _MEMPROG_STATUS_ERR_WRITE_STREAM;
                return LocalParam.Code;
            }

            if (Flush || Last || bdt.Length == BufferSize) {
                ActiveTXIdx = 0xFF;
                if (Last) {
                    TXSequence |= 0x80;
                }
                bdt.Status = MEMPROG_BUFFER_STATUS_FULL;
                bdt.Sequence = TXSequence;
                memory_sync();
                bdt.Token = MEMPROG_TOKEN_HOST;
                if (!Last) {
                    TXSequence = (TXSequence + 1) % 0x80;
                }
            }
            return 0;
        }

    private:
        inline void DEFAULT_HANDLER() { LocalParam.Status = MEMPROG_STATUS_ERR_IMPLEMENTATION;}    

        static inline void memory_sync() {
            __asm volatile("dmb");
            __asm volatile("dsb");
            __asm volatile("isb");
	    }
    
    protected:
        //	static constexpr uint8_t PIN_BUFFER1 = 0;
        //	static constexpr uint8_t PIN_BUFFER0 = 1;
        //	static constexpr uint8_t PIN_HANDLER1 = 2;
        //	static constexpr uint8_t PIN_HANDLER0 = 3;

        static void DBGSET(uint8_t pin, bool state) {
            #ifdef MEMPROG_DEBUGGING
            if (dset) {
                dset(pin, state);
            }
            #endif
        }
        
        static void DBGC(uint8_t c) {
            if constexpr (LogLevel) {
                if (dputc) {
                    dputc(c);
                }
            }
        }
        static void DBGS(const char * s) {
            if constexpr (LogLevel) {
                if (dputc) {
                    while (*s) {
                        dputc(*s);
                        s++;
                    }
                }
            }
        }

        static void DBGEND() {
            if constexpr (LogLevel) {
                DBGC('\n');
            }
        }

        static void DBGH1(uint8_t v, bool space=true) {
            if constexpr (LogLevel) {
                DBGC(hex[v >> 4]);
                DBGC(hex[v & 0x0F]);
                if (space) DBGC(' ');
            }
        }

        static void DBGH4(uint32_t v, bool space=true) {
            if constexpr (LogLevel) {
                DBGH1(v >> 24, false);
                DBGH1(v >> 16, false);
                DBGH1(v >> 8, false);
                DBGH1(v, space);
            }
        }
    };