#include "GPIODriver.h"

void GPIODriver::DriverInit() {
	#ifdef HAL_STM32F103xB
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPDEN | RCC_APB2ENR_IOPEEN;
	#else
	#error "GPIODriver not implemented for this HAL"
	#endif
}

void GPIODriver::configure(MODE mode_, uint8_t alternate_, TYPE type_, SPEED speed_, PULL pull_) const {
	uint8_t cnf = 0;
	uint8_t _mode = 0;
	uint8_t odr = 0;

	switch(mode_) {
		case MODE::INPUT: 
			switch (pull_) {
				case PULL::NONE:
					cnf |= 1;
					break;
				case PULL::UP:
					cnf |= 2;
					odr |= 1;
					break;
				case PULL::DOWN:
					cnf |= 2;
					break;
			}
			break;
		case MODE::OUTPUT:
		case MODE::ALTERNATE: 
			if (type_ == TYPE::OPEN_DRAIN) 
				cnf |= 1;
			if (mode_ == MODE::ALTERNATE)
				cnf |= 2;
			
			switch (speed_) {
				case SPEED::LOW:
					_mode |= 2;
					break;
				case SPEED::MEDIUM:
					_mode |= 1;
					break;
				case SPEED::HIGH:
				case SPEED::VERY_HIGH:
					_mode |= 3;
					break;	
			}
			break;	
		case MODE::ANALOG:
			break;
	}

	uint32_t cr = ((cnf << 2) | (_mode << 0)) << (4 * hal.offset());
	hal.CR() = (hal.CR() & ~(0xF << (4 * hal.offset()))) | cr;

	// NOTE: Alternate function selection deos not work like on other processors. It should be manually set up by the app

	if (mode_ == MODE::INPUT) {
		// ODR register reused for pull selection in input mode
		hal.gpio()->ODR = odr << hal.pin;
	}	
}

void GPIODriver::unconfigure() const {
	// Set to disable, push-pull, low speed, no pull
	// Set to:	floating input | input
	uint32_t cr = ((0x01 << 2) | (0x00 << 0)) << (4 * hal.offset());
	hal.CR() = (hal.CR() & ~(0x0F << (4 * hal.offset()))) | cr;	
}

void GPIODriver::set_physical_level(bool state) const {
	if (state) {
		hal.gpio()->BSRR = 1 << hal.pin;
	} else {
		hal.gpio()->BSRR = 1 << (hal.pin + 16);
	}
}

bool GPIODriver::get_physical_level() const {
	return ((hal.gpio()->IDR >> hal.pin) & 1);
}