#pragma once
#include "IOMAP.h"
#include "constexpr_map.h"

struct PWMHAL {
private:
	struct INFO {
		const uint8_t RCC_ENR_OFFSET;
		const uint8_t ENR_POS;
		[[nodiscard]] volatile uint32_t & ENR_REG() const { return *reinterpret_cast<uint32_t*>(RCC_BASE + RCC_ENR_OFFSET); }
	};

	static constexpr auto DATA {make_constexpr_map<uint32_t, INFO>(
		{
#if defined(HAL_STM32F427) || defined(HAL_STM32F205)
			{TIM1_BASE,  {offsetof(RCC_TypeDef, APB2ENR), RCC_APB2ENR_TIM1EN_Pos}},
			{TIM2_BASE,  {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_TIM2EN_Pos}},
			{TIM3_BASE,  {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_TIM3EN_Pos}},
			{TIM4_BASE,  {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_TIM4EN_Pos}},
			{TIM5_BASE,  {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_TIM5EN_Pos}},
			{TIM6_BASE,  {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_TIM6EN_Pos}},
			{TIM7_BASE,  {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_TIM7EN_Pos}},
			{TIM8_BASE,  {offsetof(RCC_TypeDef, APB2ENR), RCC_APB2ENR_TIM8EN_Pos}},
			{TIM9_BASE,  {offsetof(RCC_TypeDef, APB2ENR), RCC_APB2ENR_TIM9EN_Pos}},
			{TIM10_BASE, {offsetof(RCC_TypeDef, APB2ENR), RCC_APB2ENR_TIM10EN_Pos}},
			{TIM11_BASE, {offsetof(RCC_TypeDef, APB2ENR), RCC_APB2ENR_TIM11EN_Pos}},
			{TIM12_BASE, {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_TIM12EN_Pos}},
			{TIM13_BASE, {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_TIM13EN_Pos}},
			{TIM14_BASE, {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_TIM14EN_Pos}},
#elif defined(HAL_STM32G4x)
#error "PWMDriver not finished for this HAL"
			// FIXME double-check / finish this
			{TIM2_BASE,  {offsetof(RCC_TypeDef, APB1ENR1), RCC_APB1ENR1_TIM2EN}},
			{TIM3_BASE,  {offsetof(RCC_TypeDef, APB1ENR1), RCC_APB1ENR1_TIM3EN}},
			{TIM4_BASE,  {offsetof(RCC_TypeDef, APB1ENR1), RCC_APB1ENR1_TIM4EN}},
			{TIM5_BASE,  {offsetof(RCC_TypeDef, APB1ENR1), RCC_APB1ENR1_TIM5EN}},
#elif defined(HAL_STM32F103xB)
			{TIM1_BASE,  {offsetof(RCC_TypeDef, APB2ENR), RCC_APB2ENR_TIM1EN_Pos}},
			{TIM2_BASE,  {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_TIM2EN_Pos}},
			{TIM3_BASE,  {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_TIM3EN_Pos}},
			{TIM4_BASE,  {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_TIM4EN_Pos}},
#else
#error "PWMDriver not implemented for this HAL"
#endif
		})};

public:
	const uint32_t timer_address;
	const uint8_t channel;
	const uint8_t CCMR_Offset;
	const uint8_t CCER_Offset;
	const INFO info;

	PWMHAL() = delete;
	constexpr PWMHAL(uint32_t timer_address, uint8_t channel) :
	timer_address(timer_address), channel(channel),
	CCMR_Offset((channel % 2) * 8), CCER_Offset(channel * 4), info(DATA.at(timer_address)) {}

	inline TIM_TypeDef* timer() const { return reinterpret_cast<TIM_TypeDef*>(timer_address); }
	inline __IO uint32_t * CCMR() const { return reinterpret_cast<__IO uint32_t *>((&timer()->CCMR1) + (channel / 2)); }
	inline __IO uint32_t * CCR()  const { return reinterpret_cast<__IO uint32_t *>((&timer()->CCR1) + channel); }
};
