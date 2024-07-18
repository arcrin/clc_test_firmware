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
