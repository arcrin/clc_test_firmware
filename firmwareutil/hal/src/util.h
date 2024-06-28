#pragma once
#include <cstdint>

void _delay_ms(uint32_t ms);
void _delay_us(uint32_t us);
void _delay_loop(uint32_t iterations);

// wait until `reg` == `val`
// `reg` and `val` can be any expression, such as a function call
// Test the condition up to `iterations` + 1 times, with a `iteration_delay_us` microsecond delay after each failed test
// NOTE: this does not account for execution time of the condition check. Use `TIMER_TIMEOUT` for more precise timing
#define ITERATION_TIMEOUT(reg, val, iterations, iteration_delay_us) ({ \
    bool ret = true;                       \
    uint32_t t = (iterations);             \
	while ((reg) != (val))                 \
	{                                      \
		if (0 == t)                        \
		{                                  \
			ret = false;                   \
        	break;                         \
		}                                  \
		_delay_us(iteration_delay_us);     \
		t--;                               \
	}									   \
	ret;							       \
})

