#pragma once
#include "memprog.h"

// Size of each buffer chunk
#define MEMPROG_BUFFER_SIZE 0x100
// Number of buffer chunks
// With 2 or more chunks, MemProg will be able to run command handlers at the same time as transferring data
#define MEMPROG_NUM_BUFFERS 2
// Total number of MemProg interfaces used
#define MEMPROG_NUM_INTERFACES 0
// Log verbosity for debugging. `dputc` must be implemented for logging
#define MEMPROG_LOG_LEVEL MEMPROG_LOG_OFF
// Maximum time command handlers can run for before `MemProg::StaticRun will return
#define MEMPROG_HANDLER_TIMEOUT 50
// Enable debug pin usage. `dset` must be implemented for this 
// #define MEMPROG_DEBUGGING