#pragma once
#include "IOMAP.h"

struct GPIOHAL {
	const uint32_t gpio_address;
	const uint32_t pin;

	inline GPIO_TypeDef * gpio() const { return reinterpret_cast<GPIO_TypeDef *>(gpio_address);}
	inline volatile uint32_t & CR() const { return pin >= 8 ? gpio()->CRH : gpio()->CRL; }
	inline uint8_t offset() const { return pin % 8; }
};