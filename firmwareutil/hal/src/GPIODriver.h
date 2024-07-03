#pragma once
#include <cstdint>
#include "GPIOHAL.h"

class GPIODriver {
	public:
		enum class MODE : uint8_t{
			INPUT = 0,
			OUTPUT = 1,
			ALTERNATE = 2,
			ANALOG = 3
		};

		enum class TYPE : uint8_t {
			PUSH_PULL = 0,
			OPEN_DRAIN = 1
		};

		enum class SPEED : uint8_t {
			LOW = 0,
			MEDIUM = 1,
			HIGH = 2,
			VERY_HIGH = 3
		};

		enum class PULL : uint8_t {
			NONE = 0,
			UP = 1,
			DOWN = 2
		};
		
		constexpr GPIODriver(GPIOHAL hal, 
							MODE mode=MODE::INPUT, 
							uint8_t alternate = 0, 
							bool polarity=false, 
							bool initial_level=false,
							TYPE type=TYPE::PUSH_PULL,
							SPEED speed=SPEED::LOW,
							PULL pull=PULL::NONE) :
							hal(hal), 
							mode(mode),
							alternate(alternate),
							polarity(polarity),
							initial_level(initial_level),
							type(type),
							speed(speed),
							pull(pull),
							current_level(initial_level) {}

		GPIODriver() = delete;
		constexpr ~GPIODriver() = default;

		/// Common Initialization of the IO system
		static void DriverInit();

		/// (Re-)configure the pin based on settings provided in the constructor
		void configure() const;	

		/// (Re-)configure the pin based on new settings. These settings do NOT persist to future calls to configure()
		void configure(MODE mode_, uint8_t alternate_, TYPE type_, SPEED speed_, PULL pull_) const;

		/// Unconfigure the pin, resetting it back to an input
		void unconfigure() const;

		/// Immediately change the mode setting. This does not persist to future calls to configure()
		void set_mode(MODE m) const;

		void set_pull(PULL p) const;	

		void set_type(TYPE t) const;	

		void set_speed(SPEED s) const;	

		void set_alternate(uint8_t a) const;	

		/// Immediately change the polarity setting. This DOES persist to future calls to configure()
		void set_polarity(bool p);

		void set_level(bool level) const;	

		bool get_level() const;

		uint32_t get_cr() const;


	private:
		/// @brief Set the physical level of the output
		/// @param state Physical pin state
		void set_physical_level(bool state) const;

		/// @brief Get the physical pin state
		/// @return physical pin state
		bool get_physical_level() const;	

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