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
    // 
};