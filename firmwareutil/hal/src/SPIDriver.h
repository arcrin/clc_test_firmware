#pragma once
#include "GPIODriver.h"
#include "SPIConfig.h"
#include "SPIHAL.h"

/// An instance of this class represents the SPI peripheral only. Full duplex is assumed
///  The peripheral is routed to IO pins by using a SPIConfig instance
///  Each device on the SPI bus is represented by a SPIDevice instance
class SPIDriver {
public:
	/// Create an SPI instance
	/// \param hal Processor specific information about the SPI peripheral
	constexpr SPIDriver(SPIHAL hal) :
	hal(hal) {}
	SPIDriver() = delete;
	~SPIDriver() = default;

	/// Common initialization of the SPI system
	static void DriverInit();

	/// (Re-)configure the SPI peripheral using the settings provided in config, and route it to the IO pins
	///  provided in config
	/// \param config Configuration to use
	void configure(SPIConfig const & config) const;

	/// Transfer one byte
	/// \param WData The byte to clock out
	/// \param RData[out] The byte that was clocked in
	/// \return True on success. False if an error occurred
	bool TransferByte(uint8_t WData, uint8_t & RData) const;

private:
	const SPIHAL hal;
	mutable const SPIConfig *current_config {};
};
