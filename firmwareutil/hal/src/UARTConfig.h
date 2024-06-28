#pragma once
#include <cstdint>
#include "GPIODriver.h"

/// UART peripheral / bus configuration
struct UARTConfig {
	enum class PARITY : uint8_t {
		NONE=0,
		ODD=1,
		EVEN=2,
	};

	enum class STOP : uint8_t {
		ONE=0,
		TWO=1,
	};

	/// Baud rate
	uint32_t baud;
	/// Parity setting
	PARITY parity;
	/// Number of stop bits
	STOP stop;
	/// IO pin to use for TX. Must be constructed with the appropriate alternate function
	const GPIODriver TX;
	/// IO pin to use for RX. Must be constructed with the appropriate alternate function
	const GPIODriver RX;
	/// Optional IO pin to use for RS485 RTS. Must be constructed as an output
	const GPIODriver * RTS=nullptr;
};
