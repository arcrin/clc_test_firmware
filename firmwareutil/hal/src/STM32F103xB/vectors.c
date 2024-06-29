// #include <stdint.h>
// #include "vectors.h"
// #include "IOMAP.h"

// void Default_IRQHandler() {
// 	volatile uint32_t active_handler = SCB->ICSR & 0x1FF;
// 	(void)active_handler;
// 	__asm("bkpt");
// 	while(1);
// }

// extern uint8_t _estack;
// const uint32_t __vector_table[59] __attribute__ ((used)) __attribute__ ((section(".isr_vector"))) =
// {
// 		(const uint32_t) &_estack,
// 		(const uint32_t) Reset_Handler,

// 		//--------------------------------------------------------------------------
// 		//
// 		// Cortex-M core exceptions
// 		//
// 		(const uint32_t) NMI_IRQHandler,
// 		(const uint32_t) HardFault_IRQHandler,
// 		(const uint32_t) MemManage_IRQHandler,
// 		(const uint32_t) BusFault_IRQHandler,
// 		(const uint32_t) UsageFault_IRQHandler,
// 		0,                          // Reserved
// 		0,                          // Reserved
// 		0,                          // Reserved
// 		0,                          // Reserved
// 		(const uint32_t) SVC_IRQHandler,
// 		(const uint32_t) DebugMon_IRQHandler,
// 		0,                          // Reserved
// 		(const uint32_t) PendSV_IRQHandler,             // The OS context switch interrupt
// 		(const uint32_t) SysTick_IRQHandler,            // The OS timer

// 		//--------------------------------------------------------------------------
// 		//
// 		// Peripheral interrupts
// 		//
// 		(const uint32_t) WWDG_IRQHandler,
// 		(const uint32_t) PVD_IRQHandler,
// 		(const uint32_t) TAMPER_IRQHandler,
// 		(const uint32_t) RTC_IRQHandler,
// 		(const uint32_t) FLASH_IRQHandler,
// 		(const uint32_t) RCC_IRQHandler,
// 		(const uint32_t) EXTI0_IRQHandler,
// 		(const uint32_t) EXTI1_IRQHandler,
// 		(const uint32_t) EXTI2_IRQHandler,
// 		(const uint32_t) EXTI3_IRQHandler,
// 		(const uint32_t) EXTI4_IRQHandler,
// 		(const uint32_t) DMA1_Channel1_IRQHandler,
// 		(const uint32_t) DMA1_Channel2_IRQHandler,
// 		(const uint32_t) DMA1_Channel3_IRQHandler,
// 		(const uint32_t) DMA1_Channel4_IRQHandler,
// 		(const uint32_t) DMA1_Channel5_IRQHandler,
// 		(const uint32_t) DMA1_Channel6_IRQHandler,
// 		(const uint32_t) DMA1_Channel7_IRQHandler,
// 		(const uint32_t) ADC1_2_IRQHandler,
// 		(const uint32_t) USB_HP_CAN1_TX_IRQHandler,
// 		(const uint32_t) USB_LP_CAN1_RX0_IRQHandler,
// 		(const uint32_t) CAN1_RX1_IRQHandler,
// 		(const uint32_t) CAN1_SCE_IRQHandler,
// 		(const uint32_t) EXTI9_5_IRQHandler,
// 		(const uint32_t) TIM1_BRK_IRQHandler,
// 		(const uint32_t) TIM1_UP_IRQHandler,
// 		(const uint32_t) TIM1_TRG_COM_IRQHandler,
// 		(const uint32_t) TIM1_CC_IRQHandler,
// 		(const uint32_t) TIM2_IRQHandler,
// 		(const uint32_t) TIM3_IRQHandler,
// 		(const uint32_t) TIM4_IRQHandler,
// 		(const uint32_t) I2C1_EV_IRQHandler,
// 		(const uint32_t) I2C1_ER_IRQHandler,
// 		(const uint32_t) I2C2_EV_IRQHandler,
// 		(const uint32_t) I2C2_ER_IRQHandler,
// 		(const uint32_t) SPI1_IRQHandler,
// 		(const uint32_t) SPI2_IRQHandler,
// 		(const uint32_t) USART1_IRQHandler,
// 		(const uint32_t) USART2_IRQHandler,
// 		(const uint32_t) USART3_IRQHandler,
// 		(const uint32_t) EXTI15_10_IRQHandler,
// 		(const uint32_t) RTC_Alarm_IRQHandler,
// 		(const uint32_t) USBWakeUp_IRQHandler,
// };
