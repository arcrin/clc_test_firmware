#pragma once
#include "SPIDriver.h"
#include "util.h"

/// This class represents a device connected to an SPI bus
class SPIDevice {
public:
	/// Create an SPI device
	/// \param spi SPI port the device is connected to
	/// \param CS Chip select pin of the device. Must be configured as an output with logical high being the asserted state
	constexpr SPIDevice(const SPIDriver & spi, const SPIConfig & spi_config, const GPIODriver && CS) :
		spi(spi), spi_config(spi_config), CS(std::move(CS)) {}
	SPIDevice() = delete;

	/// (Re-)configure the SPI peripheral with the configuration provided in the constructor
	void configure() const;

	/// Transfer data in half-duplex mode. First WData is sent out, then '\x00' bytes are clocked out and RData is filled
	/// \param WData Data to send
	/// \param WLength Number of bytes to send
	/// \param RData Buffer to receive into
	/// \param RLength Number of bytes to read
	/// \param AssertPerByte If true, briefly de-assert CS after every byte. Otherwise, CS is asserted for the entire transfer
	/// \param Finish If true, de-assert CS at the end of the transfer. Otherwise, leave CS asserted at the end of the transfer.
	/// \return True on success. False if an error occurred
	bool TransferHalf(const uint8_t *WData, uint32_t WLength, uint8_t *RData, uint32_t RLength,
					  bool AssertPerByte=false, bool Finish=true) const;

	/// Transfer data in full-duplex mode. For each byte written, one byte is received
	/// \param WData Data to send
	/// \param Length Number of bytes to transfer
	/// \param RData Buffer to receive into. Must be large enough to receive `Length` bytes
	/// \param AssertPerByte If true, briefly de-assert CS after every byte. Otherwise, CS is asserted for the entire transfer
	/// \param Finish If true, de-assert CS at the end of the transfer. Otherwise, leave CS asserted at the end of the transfer.
	/// \return True on success. False if an error occurred
	bool TransferFull(const uint8_t *WData, uint32_t Length, uint8_t * RData,
				      bool AssertPerByte=false, bool Finish=true) const;

	// TODO these are public because they're used by SPIResource (spi.configure()) and EOResource (CS.configure()).
	//  Fix this when it's more clear how drivers/resources should interact with instances of drivers that they hold
	const SPIDriver & spi;
	const SPIConfig & spi_config;
	const GPIODriver CS;
};
