#pragma once
#include "IOMAP.h"
#include "constexpr_map.h"
#include "GPIODriver.h"

struct ADCHAL {
private:
	struct INFO {
		const uint8_t ENR_POS;
	};

	static constexpr auto DATA {make_constexpr_map<uint32_t, INFO>(
		{
#if defined(HAL_STM32F427) || defined(HAL_STM32F205)
			{ADC1_BASE, {RCC_APB2ENR_ADC1EN_Pos}},
			{ADC2_BASE, {RCC_APB2ENR_ADC2EN_Pos}},
			{ADC3_BASE, {RCC_APB2ENR_ADC3EN_Pos}},
#elif defined(HAL_STM32F103xB)
			{ADC1_BASE, {RCC_APB2ENR_ADC1EN_Pos}},
			{ADC2_BASE, {RCC_APB2ENR_ADC2EN_Pos}},
#else
#error
#endif
		})};

public:
	const uint32_t adc_address;
	const uint8_t channel;
	const GPIODriver adc_pin;
	const INFO info;

	constexpr ADCHAL(uint32_t adc_address, uint8_t channel, const GPIODriver && adc_pin) :
		adc_address(adc_address), channel(channel), adc_pin(adc_pin), info(DATA.at(adc_address))
	{}

	[[nodiscard]] ADC_TypeDef *adc() const { return reinterpret_cast<ADC_TypeDef *>(adc_address); };
};
