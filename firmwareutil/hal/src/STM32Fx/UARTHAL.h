#pragma once
#include "IOMAP.h"
#include "constexpr_map.h"

struct UARTHAL {
private:
	struct INFO {
		const uint8_t RCC_ENR_OFFSET;
		const uint8_t ENR_POS;
		// FIXME: This divider depends on the ratio between system clock and peripheral clock, which we have no way to define
		const uint8_t Divider;
		const IRQInterruptIndex IRQIndex;
#ifdef HAL_STM32F103xB
		const uint8_t AFIO_MAPR_OFFSET;
		const uint8_t REMAP_POS;
		const uint8_t REMAP_MASK;
#endif
		[[nodiscard]] volatile uint32_t & ENR_REG() const { return *reinterpret_cast<uint32_t*>(RCC_BASE + RCC_ENR_OFFSET); }
	};

	static constexpr auto DATA {make_constexpr_map<uint32_t, INFO>(
		{
#ifdef HAL_STM32F103xB
			{USART1_BASE, {offsetof(RCC_TypeDef, APB2ENR), RCC_APB2ENR_USART1EN_Pos, 1, USART1_IRQn, offsetof(AFIO_TypeDef, MAPR), 2, 1}},
			{USART2_BASE, {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_USART2EN_Pos, 2, USART2_IRQn, offsetof(AFIO_TypeDef, MAPR), 3, 1}},
			{USART3_BASE, {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_USART3EN_Pos, 1, USART3_IRQn, offsetof(AFIO_TypeDef, MAPR), 4, 3}},
#elif defined(HAL_STM32F427)
			{USART1_BASE, {offsetof(RCC_TypeDef, APB2ENR), RCC_APB2ENR_USART1EN_Pos, 2, USART1_IRQn}},
			{USART2_BASE, {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_USART2EN_Pos, 4, USART2_IRQn}},
			{USART3_BASE, {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_USART3EN_Pos, 4, USART3_IRQn}},
			{UART4_BASE,  {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_UART4EN_Pos, 4, UART4_IRQn}},
			{UART5_BASE,  {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_UART5EN_Pos, 4, UART5_IRQn}},
			{USART6_BASE, {offsetof(RCC_TypeDef, APB2ENR), RCC_APB2ENR_USART6EN_Pos, 2, USART6_IRQn}},
			{UART7_BASE,  {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_UART7EN_Pos, 4, UART7_IRQn}},
			{UART8_BASE,  {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_UART8EN_Pos, 4, UART8_IRQn}},
#elif defined(HAL_STM32F205)
			{USART1_BASE, {offsetof(RCC_TypeDef, APB2ENR), RCC_APB2ENR_USART1EN_Pos, 2, USART1_IRQn}},
			{USART2_BASE, {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_USART2EN_Pos, 4, USART2_IRQn}},
			{USART3_BASE, {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_USART3EN_Pos, 4, USART3_IRQn}},
			{UART4_BASE,  {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_UART4EN_Pos, 4, UART4_IRQn}},
			{UART5_BASE,  {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_UART5EN_Pos, 4, UART5_IRQn}},
			{USART6_BASE, {offsetof(RCC_TypeDef, APB2ENR), RCC_APB2ENR_USART6EN_Pos, 2, USART6_IRQn}},
#else
#error
#endif
		})};

public:
	const uint32_t uart_address;
	const INFO info;
#ifdef HAL_STM32F103xB
	const uint8_t remap;
	[[nodiscard]] volatile uint32_t & MAPR_REG() const { return *reinterpret_cast<uint32_t*>(AFIO_BASE + info.AFIO_MAPR_OFFSET); }
#endif

	constexpr UARTHAL(
		uint32_t uart_address
#ifdef HAL_STM32F103xB
		,uint8_t remap
#endif
		) : 
		uart_address(uart_address), 
		info(DATA.at(uart_address))
#ifdef HAL_STM32F103xB
		,remap(remap)
#endif
	{}

	[[nodiscard]] inline USART_TypeDef * uart() const { return reinterpret_cast<USART_TypeDef *>(uart_address); };
};
