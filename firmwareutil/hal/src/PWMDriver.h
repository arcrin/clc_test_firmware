#pragma once
#include "GPIODriver.h"
#include "PWMHAL.h"

/// An instance of this class represents a single PWM output
class PWMDriver {
public:
	/// Create an PWMDriver instance. The pin is not configured until configure() is called
	/// \param pin The IO pin to use
	/// \param hal Processor specific information about the timer channel that is connected to the IO pin
	constexpr PWMDriver(const GPIODriver pin, PWMHAL hal) :
	pin(pin), hal(hal)
	{}
	PWMDriver() = delete;
	constexpr ~PWMDriver() = default;

	/// Common initialization of the PWM system
	static void DriverInit();

	/// (Re-)configure the PWM output based on settings provided in the constructor
	void configure() const;

	/// Set the duty cycle
	/// \param Percent Duty cycle percentage from 0 - 100
	void Write(uint8_t Percent) const;

	// TODO configure() and Write() need to perform some common functionality regardless of the HAL.
	//  The HALs should implement configure_impl() and Write_impl() instead

private:
	const GPIODriver pin;
	PWMHAL hal;
};
