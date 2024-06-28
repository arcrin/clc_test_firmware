#pragma once
#include <cstdint>

class SYSTICK {
public:
	SYSTICK() = delete;
	~SYSTICK() = delete;

	static void DriverInit();
	static uint32_t GetTimeMS() { return TimeMS; }
	static void Increment() { TimeMS++; }

private:
	inline static volatile uint32_t TimeMS = 0;
};

// wait until `reg` == `val`
// `reg` and `val` can be any expression, such as a function call
// Test the condition at least 1 time, or until `timeout_ms` milliseconds passes, with a `iteration_delay_us`
//  microsecond delay after each failed test
#define TIMER_TIMEOUT(reg, val, timeout_ms, iteration_delay_us) ({ \
    bool ret = true;                                         \
    uint32_t end_time = SYSTICK::GetTimeMS() + (timeout_ms); \
	while ((reg) != (val))                                   \
	{                                                        \
		if (SYSTICK::GetTimeMS() >= end_time)                \
		{                                                    \
			ret = false;                                     \
        	break;                                           \
		}                                                    \
		_delay_us(iteration_delay_us);                       \
	}									                     \
	ret;							                         \
})
