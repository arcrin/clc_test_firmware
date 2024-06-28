#pragma once
#include <cstdint>

#pragma GCC push_options
#pragma GCC optimize ("-O3")

struct CriticalSection
{
public:
	CriticalSection()
	{
		uint8_t SR_reg_local;

		__asm volatile(
		"MRS R0, PRIMASK\n\t"
		"CPSID i\n\t"
		"STRB R0, %[output]"
		: [output] "=m"(SR_reg_local)
		::
		"r0");

		if (++SR_lock == 1u) {
			SR_reg = SR_reg_local;
		}

//        __asm volatile("" ::: "memory");
		__asm volatile("dsb");
		__asm volatile("isb");
	}
	~CriticalSection()
	{
		if (--SR_lock == 0u) {
			__asm volatile(
			"ldrb r0, %[input]\n\t"
			"msr PRIMASK,r0;\n\t"
			::[input] "m"(SR_reg)
			: "r0");
		}
	}

private:
    inline static volatile uint8_t SR_reg;               /* Current value of the FAULTMASK register */
    inline static volatile uint8_t SR_lock;      /* Lock */
};

#pragma GCC pop_options
