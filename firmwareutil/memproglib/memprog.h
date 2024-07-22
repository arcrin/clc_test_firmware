#pragma once
#include <stdint.h>

#define MEMPROG_MAJOR_VERSION 3
#define MEMPROG_MINOR_VERSION 0
#define MEMPROG_PATCH_VERSION 0
#define MEMPROG_VERION        ((MEMPROG_MAJOR_VERSION << 16) | ((MEMPROG_MINOR_VERSION) << 8) | (MEMPROG_PATCH_VERSION))

// API changes require major version to change. When comparing host and target version, use this mask to ignore the rest
#define MEMPROG_VERSION_MASK 0x00FF0000

// log levels
#define MEMPROG_LOG_OFF         0
#define MEMPROG_LOG_USER        1
#define MEMPROG_LOG_ERROR       10
#define MEMPROG_LOG_WARNING     20
#define MEMPROG_LOG_INFO        30
#define MEMPROG_LOG_DEBUG       40

typedef enum __attribute__((__packed__)) {
    // Inidicates that any interface is free to overwrite the params (assuming they hold the token)
    _MEMPROG_STATUS_IDLE = 0x00,
    // Set by the host to indicate to the target that params hold information about a new command
    // The target will set it ACK after copying the neecssary information
    _MEMPROG_STATUS_START = 0x01,
    // Set by the target along with the interface number to indicate that a command has been received.
    // This lets the host know that it can set status back to IDLE and start transferring data for this interface.
    _MEMPROG_STATUS_ACK = 0x02,
    
    // Values below 0x40 are reserved. DO not use. Values >= 0x40 are return statuses

    // All other status codes are set by the command handlers upon completion of a command. They indicate
    // to the host that the command is complete and return data should be read out.
    // The host will set it back to IDLE after copying necessary data from params.
    MEMPROG_STATUS_OK                   = 0x40,
    MEMPROG_STATUS_ERR_PARAM            = 0x41,
    MEMPROG_STATUS_ERR_EXECUTION        = 0x42,
    MEMPROG_STATUS_ERR_TIMEOUT          = 0x43,

    MEMPROG_STATUS_ERR_IMPLEMENTATION   = 0x7E,
    MEMPROG_STATUS_ERR_OTHER            = 0x7F,

    // 0x80 - 0xFF should only be used internally. Memprog may overwrite the original status with one of these values.
    // In this case the original status will be shifted onto the bottom of Param.Code

    // An error was detected in `GetTXStream`
    _MEMPROG_STATUS_ERR_TX_STREAM       = 0x80,
    // An error was detected in `WriteStream`
    _MEMPROG_STATUS_ERR_WRITE_STREAM    = 0x81,
    // An error was detected in `GetRXStream`
    _MEMPROG_STATUS_ERR_RX_STREAM       = 0x82,
    // An error was detected in `ReadStream`
    _MEMPROG_STATUS_ERR_READ_STREAM     = 0x83,
    // An error was detected in `ConvertStream`
    _MEMPROG_STATUS_ERR_CONVERT_STREAM  = 0x84,
} MEMPROG_STATUS;

typedef enum __attribute__((__packed__)){
    // Mass erase: Erase the entire memory range covered by this interface, so that it can be re-programmed
    // IN
    //  None
    // OUT
    //  None
    MEMPROG_CMD_MASS_ERASE = 0x00,

    // Erase range: Erase a particular range of memory covered by this interface, so that it can be re-programmed
    // IN
    //  P1: Start address
    //  P2: Length
    // OUT
    //  None
    MEMPROG_CMD_ERASE_RANGE = 0x01,

    // Program / Verify: Load data into the locations provided by the buffers. Return a CRC32 of all data programmed
    // IN
    //  Buffers: Data to program
    // OUT
    //  None
    MEMPROG_CMD_PROG = 0x11,

    // CRC: Return a single CRC32 of the data in the locations provided by the buffers
    // IN 
    //  Buffers: Addresses and lengths of sections to read and generate CRC from
    // OUT 
    //  P1: CRC32 of all data read 
    MEMPROG_CMD_CRC = 0x40,

    // Read: Fill the provided buffers with data read out of memory at the locations provided by the buffers
    // IN
    //  Buffers: Addresses and lengths of sections to read
    // OUT
    //  Buffers: Filled with data from the provided addresses
    MEMPROG_CMD_READ = 0x50,

    // Query capabilities: Used internally by the host to find addresses of the BDTs and buffers,
    // as well as the size and number of buffers, and the version of MemProgLib on the target
    // In
    //  None
    // OUT
    //  Code: MEMPROG_VERSION
    //  P1: bdt_base_address
    //  P2: buffer_base_address
    //  P3: (NumBuffers << 24) | BufferSize
    MEMPROG_CMD_QUERY_CAP = 0x7F,

} MEMPROG_CMD;

typedef enum __attribute__((__packed__)) {
    // Host is allowed to midofy params and BDTs
    MEMPROG_TOKEN_HOST = 0x00,
    // Target is allowed to modify params and BDTs
    MEMPROG_TOKEN_TARGET = 0x80,
} MEMPROG_TOKEN;

typedef struct __attribute__((__packed__)) {
    MEMPROG_TOKEN Token: 8U;
    MEMPROG_STATUS Status: 8U;
    uint8_t Interface: 8U;
    MEMPROG_CMD Command: 8U;

    uint32_t Code;

    // 6 parameters so that the total size is a power of 2, 32 bytes
    // In reverse order so that we can replace P6 with something else in the future 
    // without breaking existing code
    uint32_t P6;
    uint32_t P5;
    uint32_t P4;
    uint32_t P3;
    uint32_t P2;
    uint32_t P1;

} MEMPROG_PARAM;

typedef enum __attribute__((__packed__)) {
    MEMPROG_BUFFER_STATUS_FREE = 0x00,
    MEMPROG_BUFFER_STATUS_PENDING = 0x01,
    MEMPROG_BUFFER_STATUS_FULL = 0x02,
} MEMPROG_BUFFER_STATUS;


typedef struct __attribute__((__packed__)) {
    // Which side is allowed to access the buffer and BDT
    MEMPROG_TOKEN Token: 8U;
    // Whether the buffer is currently free, being filled, or full
    MEMPROG_BUFFER_STATUS Status: 8U;
    // Which interface the buffer is currently used by (valid if status != free)
    uint32_t Interface: 8U;
    // 0x00 - 0x7F: Sequence number. Rolls over to 0x00 after 0x7F. 0x80 indicates that this is the last buffer
    // The sequence number is set by the buffer transmitter, to ensure that the received can receive them in the correct 
    // order (since multiple buffers may be written before the other side reads them). The sequence number should be 
    // set to 0x00 for the first buffer sent out after starting a new command, and set to 0x80 for the last buffer
    // being sent out. If more than 0x80 buffers are being transmitted, roll over to 0x00
    uint8_t Sequence: 8U;

    // Used internally by memprog target to keep track of how much data has been read / written
    uint32_t _Offset;
    // Where the data was read from / where it should be written to
    uint32_t Address;
    // Amount of data in the buffer
    uint32_t Length;
} MEMPROG_BDT;

typedef struct __attribute__((__packed__)) {
    // How many TX buffers should be acquired by the target at any given time. If the command handler cannot tolerate
    // delays in waiting to acquire a buffer, increase this so that there are always idle buffers ready to use once
    // the active buffer is full
    uint8_t TargetTXBufferAffinity;
} MEMPROG_HANDLER_CONFIG;
