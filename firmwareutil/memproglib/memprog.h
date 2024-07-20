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

