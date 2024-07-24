#include "Programming.h"
#include "memprog.hpp"
#include "SYSTICK.h"
#include "FLASHProg.h"

static FLASHProg FLASH_ALGO;

volatile MEMPROG_BDT MemProg::BufferDescriptors[NumBuffers] __attribute__ ((used, section(".algo_bdt")));
volatile uint8_t MemProg::Buffers[NumBuffers][BufferSize] __attribute__ ((used, section(".algo_buffer")));
volatile MEMPROG_PARAM MemProg::Param __attribute__ ((used, section(".algo_param")));

MemProg * const MemProg::Interfaces[NumInterfaces] {
	&FLASH_ALGO,
};
uint32_t (* const volatile MemProg::time_ms)() = &SYSTICK::GetTimeMS;


#ifdef MEMPROG_DEBUGGING
static GPIODriver DEBUG[] = {
	{PTC_BASE, 6, GPIODriver::MODE::OUTPUT},
	{PTC_BASE, 7, GPIODriver::MODE::OUTPUT},
	{PTC_BASE, 5, GPIODriver::MODE::OUTPUT},
	{PTC_BASE, 4, GPIODriver::MODE::OUTPUT},
};


void dset(uint8_t pin, bool state);
void dset(uint8_t pin, bool state) {
	if (pin >= 4) {
		return;
	}
	DEBUG[pin].set_level(state);
}

void (* const MemProgDebugMixin::dset)(uint8_t, bool) = &::dset;
#else
void (* const MemProg::dset)(uint8_t, bool) = nullptr;
#endif

#if MEMPROG_LOG_LEVEL > MEMPROG_LOG_OFF
extern void dputc(uint8_t);
void (* const MemProg::dputc)(uint8_t) = ::dputc;
#else
void (* const MemProg::dputc)(uint8_t) = nullptr;
#endif


void Programming::Init() {
#ifdef MEMPROG_DEBUGGING
	for (auto & pin : DEBUG) {
		pin.configure();
	}
#endif
	MemProg::StaticInit();
}

void Programming::Task() {
	MemProg::StaticRun();
}
