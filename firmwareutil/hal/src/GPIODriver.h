#pragma once
#include <cstdint>
#include "GPIOHAL.h"

/// An instance of this class represents a single IO pin
class GPIODriver {
public:
	enum class MODE : uint8_t {
		INPUT=0,
		OUTPUT=1,
		ALTERNATE=2,
		ANALOG=3,
	};

	enum class TYPE : uint8_t {
		PUSH_PULL=0,
		OPEN_DRAIN=1
	};

	enum class SPEED : uint8_t {
		LOW=0,
		MEDIUM=1,
		HIGH=2,
		VERY_HIGH=3
	};

	enum class PULL : uint8_t {
		NONE=0,
		UP=1,
		DOWN=2,
	};

	/// Create a GPIODriver instance. The pin is not configured until configure() is called
	/// \param hal Processor specific pin information
	/// \param mode Which mode the pin will be used as
	/// \param alternate In alternate mode, selects which alternate function is used. Ignored otherwise
	/// \param polarity In input and output mode, setting this to true will invert the physical state of the pin when using [set/get]_level(). Ignored otherwise
	/// \param initial_level In output mode, this is the logical level to set the pin to when calling configure(). Ignored otherwise
	/// \param type In output mode, selects the output type. Ignored otherwise
	/// \param speed In output mode, selects the drive strength/speed of the pin. Ignored otherwise
	/// \param pull Select the pull up/down resistor
	constexpr GPIODriver(GPIOHAL hal, MODE mode=MODE::INPUT, uint8_t alternate=0, bool polarity=false,
						 bool initial_level=false, TYPE type=TYPE::PUSH_PULL, SPEED speed=SPEED::LOW, PULL pull=PULL::NONE) :
			hal(hal), mode(mode), alternate(alternate), polarity(polarity), initial_level(initial_level), type(type),
			speed(speed), pull(pull), current_level(initial_level)
	{}
	GPIODriver() = delete;
	constexpr ~GPIODriver() = default;

	/// Common initialization of the IO system
	static void DriverInit();

	/// (Re-)configure the pin based on settings provided in the constructor
	void configure() const;

	/// (Re-)configure the pin based on new settings. These settings do NOT persist to future calls to configure()
	void configure(MODE mode_, uint8_t alternate_, TYPE type_, SPEED speed_, PULL pull_) const;

	/// Unconfigure the pin, resetting it back to an input
	void unconfigure() const;

	/// Immediately change the mode setting. This does NOT persist to future calls to configure()
	void set_mode(MODE m) const;
	/// Immediately change the pull setting. This does NOT persist to future calls to configure()
	void set_pull(PULL p) const;
	/// Immediately change the type setting. This does NOT persist to future calls to configure()
	void set_type(TYPE t) const;
	/// Immediately change the speed setting. This does NOT persist to future calls to configure()
	void set_speed(SPEED s) const;
	/// Immediately change the alternate setting. This does NOT persist to future calls to configure()
	void set_alternate(uint8_t alt) const;

	/// Immediately change the polarity of the pin. This DOES persist to future calls to configure()
	/// \param pol New polarity. True = inverted
	void set_polarity(bool pol);

	/// Set the logical state of the output. The physical level may be inverted based on polarity
	/// \param state Logical pin state
	void set_level(bool state) const;

	/// Get the logical state of the pin. The physical level may be inverted based on polarity
	/// \return Logical pin state
	bool get_level() const;

private:
	/// Set the physical state of the output
	/// \param state Physical pin state
	void set_physical_level(bool state) const;
	/// Get the physical pin state
	/// \return Physical pin state
	bool get_physical_level() const;

	// TODO the mutables here prevent this class from being constexpr move-constructed. It be worth creating a wrapper
	//  class to handle logical state / polarity, so that this class can be fully constexpr
	const GPIOHAL hal;
	const MODE mode;
	const uint8_t alternate;
	mutable bool polarity;
	const bool initial_level;
	const TYPE type;
	const SPEED speed;
	const PULL pull;
	mutable bool current_level;
};
