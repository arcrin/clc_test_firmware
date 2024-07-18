#include "util.h"


#pragma GCC push_options
#pragma GCC optimize ("-O3")

void _delay_ms(uint32_t ms) {
	while (ms--) {
		_delay_us(1000);
	}
}
void _delay_loop(uint32_t iterations) {
	while (iterations--) {
		asm volatile ("nop");
	}
}

void _delay_us(uint32_t us) {
#ifdef HAL_RA6M4
	static_assert(F_CPU==48000000);
	us *= 4;
#elif defined(HAL_K2x) || defined(HAL_RA2A1)
	// NOTE tuned for K20 48 MHz -O3
	static_assert(F_CPU==48000000);
	us *= 12;
#elif defined(HAL_STM32Fx)
#if F_CPU==168000000
	us *= 27;
#elif F_CPU==120000000
	us *= 20;
#elif F_CPU==8000000
	us *= 8;
#else
#error "_delay_us not implemented for this HAL"
#endif
#else
#error "_delay_us not implemented for this HAL"
#endif
	while (us--) {
		asm volatile ("nop");
	}
}

#pragma GCC pop_options
