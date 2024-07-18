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

	uint32_t baud;
	PARITY parity;
	STOP stop;

	const GPIODriver TX; 
	const GPIODriver RX;
	const GPIODriver * RTS = nullptr;
};
