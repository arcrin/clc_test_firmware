#pragma once
#include <cstdint>
#include "SPIDevice.h"

/// This driver is processor-independent. It only relies on other drivers: SPI
/// An instance of this class represents a single AT45DB16 series dataflash chip
class DFDriver {
private:
	static constexpr uint32_t DATAFLASH_SIZE = 2 * 1024 * 1024;
	static constexpr uint32_t DATAFLASH_PAGE_SIZE = 512;

public:
	/// Create a dataflash instance
	/// \param spi SPIDevice that is connected to the dataflash chip
	constexpr DFDriver(const SPIDevice & spi) :
	spi(spi)
	{}
	DFDriver() = delete;
	~DFDriver() = default;

	/// Common initialization of the dataflash system
	static void DriverInit();

	/// (Re-)configure the dataflash SPI
	void configure() const;

	/// Write to the the dataflash and read back to verify. This will automatically erase if needed
	/// \param Address Address to start writing to
	/// \param WData Data to write
	/// \param WLength Number of bytes to write
	/// \param Retries How many times to retry if verification fails
	/// \return 0 on success
	uint64_t WriteVerify(uint32_t Address, const uint8_t * WData, uint32_t WLength, uint8_t Retries=2) const;

	/// Write to the dataflash
	/// \param Address Address to start writing to
	/// \param WData Data to write
	/// \param WLength Number of bytes to write
	/// \return True on success. False if an error occurred
	bool Write(uint32_t Address, const uint8_t * WData, uint32_t WLength) const;

	/// Read from the dataflash
	/// \param Address Address to start reading from
	/// \param RData Buffer to store the data
	/// \param RLength Number of bytes to read
	/// \return True on success. False if an error occurred
	bool Read(uint32_t Address, uint8_t * RData, uint32_t RLength) const;

	/// Read the dataflash status word
	/// \param[out] Status Dataflash status
	/// \return True on success. False if an error occurred
	bool GetStatus(uint16_t &Status) const;

	/// Read the dataflash busy bit
	/// \param[out] Busy True if the dataflash is busy
	/// \return True on success. False if an error occurred
	bool Busy(bool &Busy) const;

	/// Block until the dataflash is not busy
	/// \param poll_ms How often to check if the dataflash is busy, in milliseconds
	/// \return True on success. False if an error occurred
	bool WaitBusy(uint32_t poll_ms=0) const;

	/// Start a chip erase and optionally block until it's finished
	/// \param Wait True to block until the chip erase is finished
	/// \return True on success. False if an error occurred
	bool ChipErase(bool Wait) const;

private:
	const SPIDevice & spi;
};
