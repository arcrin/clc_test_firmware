#pragma once
#include <cstdint>
#include "GPIODriver.h"

struct UARTConfig {
	enum class PARITY : uint8_t {
		NONE = 0,
		ODD = 1,
		EVEN = 2,
	};

	enum class STOP : uint8_t {
		ONE = 0,
		TWO = 1,
	};

	constexpr UARTConfig(
		uint32_t baudrate,
		PARITY parity,
		STOP stop
	): baudrate(baudrate),
	   parity(parity),
	   stop(stop)
	{}

	uint32_t baudrate;
	PARITY parity;
	STOP stop;
};
