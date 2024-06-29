#pragma once
#include "macro_enum_map.h"
#include <string_view>

#define ERR_MAP_GET_VALUE(key, str) str,
ENUM_MAP(enum class SHELLERR : uint8_t, constexpr std::string_view SHELLERRStrings[], 1, ERR_MAP_GET_VALUE, 
    OK,             "",
    IMPLEMENTATION, "IMPLEMENTATION - NOT IMPLEMENTED",
    EXECUTION,      "EXECUTION - COMMAND FAILED",
    RESOURCE,       "RESOURCE - NO SUCH RESOURCE",
    ARGUMENT,       "ARGUMENT - BAD ARGUMENT",
    CHECKSUM,       "CHECKSUM - COMMAND/RESPONSE CHECKSUM FAILURE",
);

// The above macro expands to:

// enum class SHELLERR : uint8_t{ 
//     OK, 
//     IMPLEMENTATION, 
//     EXECUTION, 
//     RESOURCE, 
//     ARGUMENT, 
//     CHECKSUM, 
//     }; 
// constexpr std::string_view SHELLERRStrings[]{ 
//     "",
//     "IMPLEMENTATION - NOT IMPLEMENTED", 
//     "EXECUTION - COMMAND FAILED", 
//     "RESOURCE - NO SUCH RESOURCE",
//     "ARGUMENT - BAD ARGUMENT",
//     "CHECKSUM - COMMAND/RESPONSE CHECKSUM FAILURE", 
//     };