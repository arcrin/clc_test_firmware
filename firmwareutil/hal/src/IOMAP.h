#ifndef HAL_IOMAP_H
#define HAL_IOMAP_H

typedef enum
{
/******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
	NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                                          */
	MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M4 Memory Management Interrupt                           */
	BusFault_IRQn               = -11,    /*!< 5 Cortex-M4 Bus Fault Interrupt                                   */
	UsageFault_IRQn             = -10,    /*!< 6 Cortex-M4 Usage Fault Interrupt                                 */
	SVCall_IRQn                 = -5,     /*!< 11 Cortex-M4 SV Call Interrupt                                    */
	DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M4 Debug Monitor Interrupt                              */
	PendSV_IRQn                 = -2,     /*!< 14 Cortex-M4 Pend SV Interrupt                                    */
	SysTick_IRQn                = -1,     /*!< 15 Cortex-M4 System Tick Interrupt                                */
} IRQn_Type;

#if defined(HAL_K20) || defined(HAL_K24)
#define __MPU_PRESENT             0U
#define __Vendor_SysTickConfig    0U
#define __FPU_PRESENT             0U
#define __NVIC_PRIO_BITS          4U

#include "core_cm4.h"             /* Cortex-M4 processor and core peripherals */
#endif



#ifdef HAL_K20
#include "MK20D7.h"
#elif defined(HAL_K24)
#include "MK24F12.h"
#elif defined(HAL_RA2A1)
#define __ARM_ARCH_7EM__ 0
#define __ARM_ARCH_6M__ 0
#define __ARM_ARCH_8M_BASE__ 1
#define __ARM_ARCH_8M_MAIN__ 1
#include "stdint.h"
#include "renesas.h"
#elif defined(HAL_RA6M4)
#define __ARM_ARCH_7EM__ 0
#define __ARM_ARCH_6M__ 0
#define __ARM_ARCH_8M_BASE__ 0
#define __ARM_ARCH_8M_MAIN__ 1
#include "stdint.h"
#include "renesas.h"
#elif defined(HAL_STM32F1x)
#define __FPU_PRESENT             0U
#include "stm32f1xx.h"
#elif defined(HAL_STM32F2x)
#define __FPU_PRESENT             0U
#include "stm32f2xx.h"
#elif defined(HAL_STM32F4x)
#include "stm32f4xx.h"
#elif defined(HAL_STM32G4x)
#include "stm32g4xx.h"
#else
#error "Bad HAL selection"
#endif

#ifdef HAL_K2x
inline IRQn_Type ToIRQ(IRQInterruptIndex index) { return (IRQn_Type)(index - 16); }
#elif defined(HAL_RAx)
// NOTE ToIRQ must be implemented by the app
//inline IRQn_Type ToIRQ(IRQInterruptIndex index)  {
//}
#elif defined(HAL_STM32x)
inline IRQn_Type ToIRQ(IRQInterruptIndex index) { return (IRQn_Type)(index); }
#endif

#include <assert.h>
#include <stdlib.h>

#endif //HAL_IOMAP_H
