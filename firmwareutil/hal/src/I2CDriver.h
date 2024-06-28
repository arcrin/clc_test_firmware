#pragma once
#include <cstdint>
#include "GPIODriver.h"

/// This driver is processor-independent. It only relies on other drivers: GPIODriver
/// An instance of this class represents an I2C port
class I2CDriver {
public:
	/// Create an I2C instance
	/// \param SCL GPIODriver used for SCL. It must be configured as open-drain output
	/// \param SDA GPIODriver used for SDA. It must be configured as open-drain output
	/// \param ClockDelayus Number of microseconds between clock transitions
	constexpr I2CDriver(const GPIODriver SCL, const GPIODriver SDA, uint32_t ClockDelayus=5) :
		SCL(SCL), SDA(SDA), ClockDelayus(ClockDelayus) {}
	I2CDriver() = delete;
	~I2CDriver() = default;

	/// Common initialization of the I2C system
	static void DriverInit() {};

	void configure() const;
	bool WriteRead(uint8_t Address, const uint8_t * WData, uint32_t WriteLen, const uint8_t *RData, uint32_t NumBytes) const;
	bool Write(uint8_t Address, const uint8_t * Data, uint32_t NumBytes) const;
	bool Read(uint8_t Address, const uint8_t * Data, uint32_t NumBytes) const;

private:
	// FIXME This delay is supposed to be in microseconds, but is not accurate at all. On eZNT (48 MHz), 200000 is about 1 second
	static constexpr uint32_t BusTimeout = 200000;
	enum class RW : uint8_t {
		WRITE=0,
		READ=1
	};

	const GPIODriver SCL;
	const GPIODriver SDA;
	uint32_t ClockDelayus;

	bool TransferBytes(RW rw, uint8_t Address, const uint8_t * Data, uint32_t NumBytes) const;
	void Delay() const;
	[[nodiscard]] bool Start(uint8_t Address, RW rw) const;
	[[nodiscard]] bool Restart(uint8_t Address, RW rw) const;
	void Stop() const;
	uint8_t Transfer(RW rw, uint8_t Param, bool &Result) const;
	[[nodiscard]] bool SendACK(bool ACK) const;
	[[nodiscard]] bool WaitACK() const;
};
