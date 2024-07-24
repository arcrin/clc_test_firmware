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

	// NOTE ============= These must be defined in a source file in the MCU firmware
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

	// Optional debug putc function.
	static void (* const dputc)(uint8_t c);
	// Optional debug pin write function.
	static void (* const dset)(uint8_t pin, bool state);
	// NOTE ========================================================================

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

#ifndef MEMPROG_HANDLER_TIMEOUT_MS
#define MEMPROG_HANDLER_TIMEOUT_MS 30
#endif
	// Maximum duration `StaticRun` should run for
	static constexpr uint32_t HANDLER_TIMEOUT_MS           = MEMPROG_HANDLER_TIMEOUT_MS;
	// Default number of buffers `GetTXStream` should try to acquire
	static constexpr uint8_t DEFAULT_TX_BUFFER_AFFINITY    = 1;

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
			auto & inst = Interfaces[i];
			inst->Interface = i; // NOTE: each interface is assigned an `Interface` number that is the same as its index in the MemProg::Interfaces array 
			inst->Init();
		}

		Param.Status = _MEMPROG_STATUS_IDLE;
		ReleaseToken();
	}

	static void StaticRun() {
		// if we have the token
		if (TryAcquireToken()) {
			bool released = false;

			if constexpr (LogLevel >= MEMPROG_LOG_DEBUG) {
				DBGS("sr ");
				DBGH1(Param.Status); // NOTE: `Param` is a static data structure in the memory, not instance-specific
				DBGH1(Param.Interface);
				DBGEND();
			}

			// Check if host wants to start a command
			if (Param.Status == _MEMPROG_STATUS_START) {
				auto & inst = Interfaces[Param.Interface]; // NOTE: `Param.Interface` is a number that indicate the index of MemProg object in the Interfaces array 
				// Copy the volatile params to LocalParams
				inst->Active = true; // NOTE: we are starting to deal with individual MemProg instances
				memcpy(&inst->LocalParam, (const void *)&Param, sizeof(LocalParam)); // NOTE: shallow copy of the `Param` to `LocalParam`

				if constexpr (LogLevel >= MEMPROG_LOG_INFO) {
					DBGH1(inst->Interface);
					DBGS("start ");
					DBGH1(inst->LocalParam.Status);
					DBGH1(inst->LocalParam.Command);
					DBGH4(inst->LocalParam.Code);
					DBGH4(inst->LocalParam.P1);
					DBGH4(inst->LocalParam.P2);
					DBGH4(inst->LocalParam.P3);
					DBGH4(inst->LocalParam.P4);
					DBGH4(inst->LocalParam.P5);
					DBGH4(inst->LocalParam.P6);
					DBGEND();
				}

				// Check if a handler for this command exists
				if (!(inst->CurrentHandler = inst->BaseGetHandler(Param.Command))) {
					inst->LocalParam.Status = MEMPROG_STATUS_ERR_IMPLEMENTATION;
				} else { // NOTE: command handler exists 
					inst->LocalParam.Code = 0;

					inst->ActiveTXIdx = 0xFF; // Invalid value
					inst->TXSequence = 0;
					inst->ActiveRXIdx = 0xFF; // Invalid value
					inst->RXSequence = 0;

					// Set up default configuration
					inst->HandlerConfig.TargetTXBufferAffinity = DEFAULT_TX_BUFFER_AFFINITY;

					// Call handler to modify configuration data
					// Status will be START the first time this is called. Handlers can use this fact to reset their state
					(inst->*inst->CurrentHandler)(); // NOTE: this is where CMD_PROG_VERIFY() and PGM() is called

					if (inst->LocalParam.Status == _MEMPROG_STATUS_ACK) {
						// Handler returns ACK to set configuration data
						inst->HandlerConfig.TargetTXBufferAffinity = std::max(1UL, std::min(NumBuffers, inst->LocalParam.P1));

						memcpy((void *)&Param, &inst->LocalParam, sizeof(MEMPROG_PARAM));
					}

					// After the first run, change Status to something else, unless it has already changed to a valid return value
					if (inst->LocalParam.Status < MEMPROG_STATUS_OK) {
						// Setting `Code` indicates a failure, so change the status to MEMPROG_STATUS_ERR_OTHER
						inst->LocalParam.Status = inst->LocalParam.Code ? MEMPROG_STATUS_ERR_OTHER : _MEMPROG_STATUS_IDLE;
					}
				}

				// Acknowledge the command by changing status to IDLE and passing token back after copying Params
				Param.Status = _MEMPROG_STATUS_ACK;
				ReleaseToken();
				released = true;

			} else if (Param.Status == _MEMPROG_STATUS_IDLE) {
				// If not, check if anything needs to be returned
				for (auto & inst : Interfaces) {
					// if status has been set, the command has finished; notify the host by modifying Param.Status
					if (inst->Active && inst->LocalParam.Status >= MEMPROG_STATUS_OK) {
						if constexpr (LogLevel >= MEMPROG_LOG_INFO) {
							DBGH1(inst->Interface);
							DBGS("finish ");
							DBGH1(inst->LocalParam.Status);
							DBGH1(inst->LocalParam.Command);
							DBGH4(inst->LocalParam.Code);
							DBGH4(inst->LocalParam.P1);
							DBGH4(inst->LocalParam.P2);
							DBGH4(inst->LocalParam.P3);
							DBGH4(inst->LocalParam.P4);
							DBGH4(inst->LocalParam.P5);
							DBGH4(inst->LocalParam.P6);
							DBGEND();
						}

						inst->Active = false;
						inst->CurrentHandler = nullptr;

//						LocalParam.Token = MEMPROG_TOKEN_TARGET; This is necessarily already TOKEN_TARGET based on the first if statement in this function

						memcpy((void *)&Param, &inst->LocalParam, sizeof(MEMPROG_PARAM));
						// Params.Token must be changed after all the other params. It indicates to the host
						// that all other params are valid to read
						ReleaseToken();
						released = true;
						break;
					}
				}
			} else {
				// host may accidentally pass us the token before reading out return data. Do nothing in this case
				if constexpr (LogLevel >= MEMPROG_LOG_ERROR) {
					DBGS("BAD STATUS ");
					DBGH1(Param.Status);
					DBGEND();
				}
			}

			if (!released) {
				ReleaseToken();
			}
		}

		// then run any active handlers
		uint32_t overall_start_time = time_ms();
		for (auto & inst : Interfaces) {
			if (!inst->Active || inst->LocalParam.Status >= MEMPROG_STATUS_OK) {
				continue;
			}

			// if status hasn't been set yet, keep running the command
//			DBGSET(PIN_HANDLER0 - i, true);
			uint32_t current_start_time = time_ms();
			(inst->*inst->CurrentHandler)();
//			DBGSET(PIN_HANDLER0 - i, false);

			if (inst->LocalParam.Status >= MEMPROG_STATUS_OK) {
//				if constexpr (LogLevel >= MEMPROG_LOG_INFO) {
//					DBGH1(inst->Interface);
//					DBGS("finish ");
//					DBGH1(inst->LocalParam.Command);
//					DBGEND();
//				}

			} else if (inst->LocalParam.Code) {
				// Setting `Code` indicates a failure, so change the status if it's not already a valid return value
				inst->LocalParam.Status = MEMPROG_STATUS_ERR_OTHER;
			}

			uint32_t elapsed = time_ms() - current_start_time;
			if constexpr (LogLevel >= MEMPROG_LOG_WARNING) {
				if (elapsed > HANDLER_TIMEOUT_MS) {
					DBGS("OV ");
					DBGH1(inst->Interface);
					DBGH4(elapsed);
					DBGEND();
				}
			}

			elapsed = time_ms() - overall_start_time;
			if (elapsed > HANDLER_TIMEOUT_MS) {
				if constexpr (LogLevel >= MEMPROG_LOG_WARNING) {
					DBGS("OV ");
					DBGH4(elapsed);
					DBGEND();
				}
				break;
			}
		}

		// release any unused buffers
		PassBuffers();
	}

protected:
	virtual void Init() {}

	/// This method can be overridden to use a custom handler for a given command code.
	/// \param Command Command code received from host
	/// \return Pointer to the method to use as a command handler
	/// Return nullptr if the provided command code should use the default (built-in) handler.
	virtual CMD_FUNC GetHandler(MEMPROG_CMD Command) {
		return nullptr;
	}

	// stubs for global commands
	virtual inline void CMD_MASS_ERASE() { DEFAULT_HANDLER(); }
	virtual inline void CMD_ERASE_RANGE() { DEFAULT_HANDLER(); }
	virtual inline void CMD_PROG_VERIFY() { DEFAULT_HANDLER(); }
	virtual inline void CMD_PROG() { DEFAULT_HANDLER(); }
	virtual inline void CMD_CRC() { DEFAULT_HANDLER(); }
	virtual inline void CMD_READ() { DEFAULT_HANDLER(); }

	// The following functions are used in command handlers to send and receive data.
	// Any non-zero return value indicates an error, and these shall be caught be command handlers and cause
	// them to stop execution. In case the command handler is malformed and ignores the retun value, these
	// functions also set `LocalParam.Code` if an error occurs. This will cause memprog to abort the
	// command handler the next time it returns.

	/// Attempt to allocate a transmission buffer where data can be written. If no buffer is currently available,
	/// Ptr will be set to nullptr
	/// \param[out] Ptr Pointer to the next free byte where data can be written
	/// \param[in] Address Target memory address of the next byte which will be written to the stream
	/// \param[out] Length Number of consecutive bytes that can be written to the pointer
	/// \param[in] MinLength Minimum number of consecutive bytes that must be available. If the current buffer does
	/// not have enough space, it will be flushed a new buffer will be acquired
	/// \return 0 on success
	uint32_t GetTXStream(uint8_t *&Ptr, uint32_t Address, uint32_t &Length, uint32_t MinLength=0) {
		if (TXSequence & 0x80) {
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

		uint8_t NeededBuffers = HandlerConfig.TargetTXBufferAffinity; // TODO: what decides the TargetTXBufferAffinity?
		// Loop through all the buffers, acquiring free ones as needed
		for (uint8_t i = 0; i < NumBuffers && NeededBuffers; i++) {
			volatile MEMPROG_BDT &bdt = BufferDescriptors[i];
			if (bdt.Token != MEMPROG_TOKEN_TARGET) {
				continue;
			}
			memory_sync();

			if (bdt.Interface == Interface && bdt.Status == MEMPROG_BUFFER_STATUS_PENDING) {
				// This is one of our TX buffers
				// NOTE: this means the buffer has already been acquired

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
			// TODO: what?
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

		Ptr = (uint8_t *) &Buffers[ActiveTXIdx][bdt.Length];
		Length = BufferSize - bdt.Length;
		return 0;
	}

	/// Notify Memprog that data has been written to the pointer returned by `GetTXStream`
	/// \param[in] Length Number of bytes written
	/// \param[in] Last Indicate to the host that no further data will be transmitted. It is necessary to set this
	/// during the last call to `WriteStream`
	/// \param[in] Flush Force the current TX buffer to be sent to the host
	/// \return 0 on success
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

	/// Check if there is incoming data. If no data is available, Ptr will be set to nullptr
	/// \param[out] Ptr Pointer to the next incoming byte
	/// \param[out] Address Target memory address where the next incoming byte should be written to
	/// \param[out] Length Number of consecutive bytes that can be read from the pointer
	void GetRXStream(const uint8_t * &Ptr, uint32_t &Address, uint32_t &Length) {
		if (ActiveRXIdx == 0xFF) {
			// No active RX buffer, find a new one
			for (uint8_t i = 0; i < NumBuffers; i++) {
				volatile MEMPROG_BDT &bdt = BufferDescriptors[i];
				if (bdt.Token != MEMPROG_TOKEN_TARGET) { // NOTE: look for a BDT that is intended for TARGET, I assume this target means the MCU, and host means the PC
					continue;
				}
				memory_sync();
				if (bdt.Status == MEMPROG_BUFFER_STATUS_FULL && bdt.Interface == Interface &&
					((bdt.Sequence & 0x7F) == RXSequence)) {

					// Found an incoming full buffer, set it as the active buffer, initialize the offset, set the
					// next expected RXSequence
					ActiveRXIdx = i;
					bdt._Offset = 0;
					if (bdt.Sequence & 0x80) {
						RXSequence = bdt.Sequence;
					} else {
						RXSequence = (RXSequence + 1) % 0x80;
					}
					break;
				}
			}

			if (ActiveRXIdx == 0xFF) {
				// No active RX buffer
				Ptr = nullptr;
				Address = 0;
				Length = 0;
				return;
			}
		}

		volatile MEMPROG_BDT &bdt = BufferDescriptors[ActiveRXIdx];
		Ptr = (const uint8_t*) &Buffers[ActiveRXIdx][bdt._Offset];
		Address = bdt.Address + bdt._Offset;
		Length = bdt.Length - bdt._Offset;
	}

	/// Notify Memprog that data has been read from the pointer returned by `GetRXStream`
	/// \param[in] Length Number of bytes read
	/// \param[out] Last Will be set to true if all data has been read from the stream
	/// \return 0 on success
	uint32_t ReadStream(uint32_t Length, bool &Last) {
		if (ActiveRXIdx == 0xFF) {
			// Attempt to read when there is no active RX buffer
			LocalParam.Code = 1;
			LocalParam.Status = _MEMPROG_STATUS_ERR_READ_STREAM;
			return LocalParam.Code;
		}

		volatile MEMPROG_BDT &bdt = BufferDescriptors[ActiveRXIdx];
		bdt._Offset += Length;
		if (bdt._Offset > bdt.Length) {
			// read too much
			LocalParam.Code = 2;
			LocalParam.Status = _MEMPROG_STATUS_ERR_READ_STREAM;
			return LocalParam.Code;
		}
		if (bdt._Offset == bdt.Length) {
			Last = RXSequence & 0x80;
			// Release buffer
			bdt.Status = MEMPROG_BUFFER_STATUS_FREE;
			memory_sync();
			bdt.Token = MEMPROG_TOKEN_HOST;

			ActiveRXIdx = 0xFF;
		}
		return 0;
	}

	/// Convert the current RX buffer into a TX buffer
	/// \param[out] TXPtr Pointer to the next free byte where data can be written
	/// \param[in] Address Target memory address of the next byte which will be written to the stream
	/// \param[out] Length Number of consecutive bytes that can be written to the pointer
	/// \param[out] Last Will be set to true if all data has been read from the stream
	/// \return Same return values as `GetTXStream`
	uint32_t ConvertStream(uint8_t *&Ptr, uint32_t &Address, uint32_t &Length, bool &Last) {
		// Converting a buffer from RX to TX is as simple as changing the Status from FULL to PENDING and setting
		// up the other fields. However, we would need to make sure to keep the ActiveXXIdx in sync.
		// Easier to just release the buffer and acquire a new one, however this could lead to us losing the buffer
		// to the host. Instead of fully releasing the buffer (passing token back to host), just clear it, but keep
		// the token. Then `GetTXStream` will be able to grab it right away
		uint32_t Dummy, Result;

		GetRXStream(const_cast<const uint8_t* &>(Ptr), Address, Length);
		if (!Ptr) {
			return 0;
		}

		// Minimal copy of `ReadStream`
		volatile MEMPROG_BDT &bdt = BufferDescriptors[ActiveRXIdx];
		Last = RXSequence & 0x80;
		// Release buffer
		bdt.Status = MEMPROG_BUFFER_STATUS_FREE;
		ActiveRXIdx = 0xFF;

		// Now let `GetTXStream` re-acquire this buffer (or another free one) for transmission
		Result = GetTXStream(Ptr, Address, Dummy, Length);
		if (Result) {
			LocalParam.Code = (Result << 4) | 2;
			LocalParam.Status = _MEMPROG_STATUS_ERR_READ_STREAM;
			return LocalParam.Code;
		} else if (Dummy < Length) {
			LocalParam.Code = 3;
			LocalParam.Status = _MEMPROG_STATUS_ERR_READ_STREAM;
			return LocalParam.Code;
		}
		return 0;
	}

	static void PassBuffers() {
		// Pass the token of any unused buffers
		uint8_t i;
		for (i = 0; i < NumBuffers; i++) {
			volatile MEMPROG_BDT &bdt = BufferDescriptors[i];
			if (bdt.Token != MEMPROG_TOKEN_TARGET) {
				continue;
			}

			if (bdt.Interface >= NumInterfaces) {
				// Interface number should never be >= NumInterfaces
				if constexpr (LogLevel >= MEMPROG_LOG_ERROR) {
					DBGS("CORRUPT ");
					DBGH1(i);
					DBGH1(bdt.Interface);
					DBGH1(bdt.Status);
					DBGEND();
				}
				// Free the buffer so host doesn't try to receive from it
				bdt.Status = MEMPROG_BUFFER_STATUS_FREE;

			} else if (bdt.Status != MEMPROG_BUFFER_STATUS_FREE && !Interfaces[bdt.Interface]->Active) {
				// NOTE: this is an orphaned buffer
				if constexpr (LogLevel >= MEMPROG_LOG_ERROR) {
					DBGS("ORPHAN ");
					DBGH1(i);
					DBGH1(bdt.Interface);
					DBGH1(bdt.Status);
					DBGEND();
				}
				// Free the buffer so host doesn't try to receive from it
				bdt.Status = MEMPROG_BUFFER_STATUS_FREE;
			}

			if (bdt.Status == MEMPROG_BUFFER_STATUS_FREE) {
				memory_sync();
				bdt.Token = MEMPROG_TOKEN_HOST;
			}
		}
	}

	/// Create / Update a CRC32 digest
	/// \param Data Pointer to the data to hash
	/// \param Length Length of `Data`
	/// \param LastCRC The digest to update. Ignore for creating a new digest
	/// \return Digest of `Data`
	static uint32_t CalculateCRC32(const uint8_t *Data, uint32_t Length, uint32_t LastCRC= 0) {
		int8_t i;
		uint32_t Mask;

		uint32_t crc = ~LastCRC;
		while (Length--) {
			crc = crc ^ *Data;
			Data++;
			for (i = 7; i >= 0; i--) {
				Mask = -(crc & 1);
				crc = (crc >> 1) ^ (0xEDB88320 & Mask);
			}
		}
		return ~crc;
	}

private:
	inline void DEFAULT_HANDLER() { LocalParam.Status = MEMPROG_STATUS_ERR_IMPLEMENTATION; }

	void CMD_QUERY_CAP() {
		LocalParam.Code = MEMPROG_VERSION;
		LocalParam.P1 = (uint32_t)BufferDescriptors;
		LocalParam.P2 = (uint32_t)Buffers;
		LocalParam.P3 = (NumBuffers << 24) | BufferSize;
		LocalParam.Status = MEMPROG_STATUS_OK;
	}

	CMD_FUNC BaseGetHandler(MEMPROG_CMD Command) {
		switch (Command) {
			// Static commands
			case MEMPROG_CMD_QUERY_CAP:
				return &MemProg::CMD_QUERY_CAP;

			// Global commands
			case MEMPROG_CMD_MASS_ERASE:
				return &MemProg::CMD_MASS_ERASE;
			case MEMPROG_CMD_ERASE_RANGE:
				return &MemProg::CMD_ERASE_RANGE;
			case MEMPROG_CMD_PROG_VERIFY:
				return &MemProg::CMD_PROG_VERIFY;
			case MEMPROG_CMD_PROG:
				return &MemProg::CMD_PROG;
			case MEMPROG_CMD_CRC:
				return &MemProg::CMD_CRC;
			case MEMPROG_CMD_READ:
				return &MemProg::CMD_READ;

			// Interface-specific commands
			default:
				return GetHandler(Command);
		}
	}

	static inline void memory_sync() {
		__asm volatile("dmb");
		__asm volatile("dsb");
		__asm volatile("isb");
	}

	static bool TryAcquireToken() {
//		DBGSET(PIN_ACQUIRE, true);

		if (Param.Token == MEMPROG_TOKEN_TARGET) {
//			DBGSET(PIN_ACQUIRE, false);
			if constexpr (LogLevel >= MEMPROG_LOG_DEBUG) {
				DBGS("at\n");
			}
//			DBGSET(PIN_TOKEN, true);
			return true;
		}
//		DBGSET(PIN_ACQUIRE, false);
		return false;
	}

	static void ReleaseToken() {
		if constexpr (LogLevel >= MEMPROG_LOG_DEBUG) {
			DBGS("rt\n");
		}
		memory_sync();
		Param.Token = MEMPROG_TOKEN_HOST;
//		DBGSET(PIN_TOKEN, false);
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
	static void DBGS(const char* s) {
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
	static void DBGH1(uint8_t v, bool space= true) {
		if constexpr (LogLevel) {
			DBGC(hex[v >> 4]);
			DBGC(hex[v & 0x0F]);
			if (space) DBGC(' ');
		}
	}
	static void DBGH4(uint32_t v, bool space= true) {
		if constexpr (LogLevel) {
			DBGH1(v >> 24, false);
			DBGH1(v >> 16, false);
			DBGH1(v >> 8, false);
			DBGH1(v, space);
		}
	}
};
