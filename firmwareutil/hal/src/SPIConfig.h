#pragma once
#include <cstdint>
#include "GPIODriver.h"

/// SPI peripheral / bus configuration
struct SPIConfig {
	/// Transfer frequency
	uint32_t bitrate;
	/// SCK IO pin to use. Must be constructed with the appropriate alternate function
	const GPIODriver SCK;
	/// MOSI IO pin to use. Must be constructed with the appropriate alternate function
	const GPIODriver MOSI;
	/// MISO IO pin to use. Must be constructed with the appropriate alternate function
	const GPIODriver MISO;
	/// Clock phase
	bool cpha;
	/// Clock polarity
	bool cpol;
};
