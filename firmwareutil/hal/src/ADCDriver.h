#pragma once
#include "ADCHAL.h"

/// An instance of this class represents a single ADC channel
class ADCDriver {
public:
	/// Create an ADC channel
	/// \param hal Processor specific ADC channel information
	constexpr ADCDriver(const ADCHAL hal) :
	hal(hal) {}
	ADCDriver() = delete;
	~ADCDriver() = default;

	/// Common initialization of the entire ADC system
	static void DriverInit();

	/// (Re-)configure the channel
	void configure() const;

	/// Read one ADC channel
	/// \param channel ADC channel to read
	/// \param[out] result Measured ADC value
	/// \return True on success
	bool Read(uint16_t &result) const;

private:
	const ADCHAL hal;
};


