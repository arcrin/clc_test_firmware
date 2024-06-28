#include <string.h>
#include <stdint.h>
#include "IOMAP.h"

// From linker script
extern uint8_t __bss_start;
extern uint8_t __bss_end;
extern uint8_t _sstack;
extern uint8_t _estack;
extern uint8_t _eheap;
extern void (*__preinit_array_start []) (void);
extern void (*__preinit_array_end []) (void);
extern void (*__init_array_start []) (void);
extern void (*__init_array_end []) (void);

// From main.cpp
extern int main();

extern void __init_hardware(void);
extern const uint32_t __vector_table[];

// ?
__attribute__ ((weak)) void _init();

static void __libc_init_array(void);
static void __libc_init_array(void) {
	size_t count;
	size_t i;

	count = __preinit_array_end - __preinit_array_start;
	for (i = 0; i < count; i++) {
		__preinit_array_start[i]();
	}

	_init();

	count = __init_array_end - __init_array_start;
	for (i = 0; i < count; i++) {
		__init_array_start[i]();
	}
}

void startup(void);
void __attribute__((used)) startup(void) {
	SCB->VTOR = (uint32_t)__vector_table;

	__init_hardware();

	memset(&__bss_start, 0, (size_t)(&__bss_end - &__bss_start));                // zero-fill uninitialized variables
	__libc_init_array();                                            // low-level init & ctor loop

	main();
}

void Reset_Handler() __attribute__ ((section(".Reset_Handler")));
void __attribute__((naked)) Reset_Handler(void) {
	__asm(
		// set stack pointer
#ifdef HAL_RA2A1
		"ldr r0, =%0\n\t"
		"mov sp, r0\n\t"
#else
		"ldr sp, =%0\n\t"
#endif

		// fill stack and heap with 0xA5 pattern
		"ldr r3, =%2\n\t"
		"ldr r1, =%3\n\t"
		"ldr r2, =%4\n\t"
		"loop%=:\n\t"
		"str r1, [r3]\n\t"
		"add r3, r3, #4\n\t"
		"cmp r3, r2\n\t"
		"bne loop%=\n\t"
		"movs r0, #0\n\t"

		// jump to startup
		"ldr r3, =%1\n\t"
		"bx r3\n\t"

		::
		"X" (&_estack),
		"X" (startup),
		"X" (&_sstack),
		"X" (0xA5A5A5A5),
		"X" (&_eheap)
		: "r0", "r1", "r2", "r3"
		);
}
