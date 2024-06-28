#include "SYSTICK.h"
#include "IOMAP.h"

void SYSTICK::DriverInit() {
	SysTick_Config(F_CPU/1000);
}

extern "C" {
	extern void SysTick_IRQHandler();
	void SysTick_IRQHandler() {
		SYSTICK::Increment();
	}
}
