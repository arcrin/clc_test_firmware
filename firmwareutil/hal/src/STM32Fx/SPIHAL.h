#pragma once
#include "IOMAP.h"
#include "constexpr_map.h"

struct SPIHAL {
private:
	struct INFO {
		const uint8_t RCC_ENR_OFFSET;
		const uint8_t ENR_POS;
		[[nodiscard]] volatile uint32_t & ENR_REG() const { return *reinterpret_cast<uint32_t*>(RCC_BASE + RCC_ENR_OFFSET); }
	};

	static constexpr auto DATA {make_constexpr_map<uint32_t, INFO>(
		{
#ifdef HAL_STM32F427
			{SPI1_BASE, {offsetof(RCC_TypeDef, APB2ENR), RCC_APB2ENR_SPI1EN_Pos}},
			{SPI2_BASE, {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_SPI2EN_Pos}},
			{SPI3_BASE, {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_SPI3EN_Pos}},
			{SPI4_BASE, {offsetof(RCC_TypeDef, APB2ENR), RCC_APB2ENR_SPI4EN_Pos}},
			{SPI5_BASE, {offsetof(RCC_TypeDef, APB2ENR), RCC_APB2ENR_SPI5EN_Pos}},
			{SPI6_BASE, {offsetof(RCC_TypeDef, APB2ENR), RCC_APB2ENR_SPI6EN_Pos}},
#elif defined(HAL_STM32F205)
			{SPI1_BASE, {offsetof(RCC_TypeDef, APB2ENR), RCC_APB2ENR_SPI1EN_Pos}},
			{SPI2_BASE, {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_SPI2EN_Pos}},
			{SPI3_BASE, {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_SPI3EN_Pos}},
#elif defined(HAL_STM32F103xB)
			{SPI1_BASE, {offsetof(RCC_TypeDef, APB2ENR), RCC_APB2ENR_SPI1EN_Pos}},
			{SPI2_BASE, {offsetof(RCC_TypeDef, APB1ENR), RCC_APB1ENR_SPI2EN_Pos}},
#else
#error
#endif
		})};

public:
	const uint32_t spi_address;
	const INFO info;

	constexpr SPIHAL(uint32_t spi_address) :
	spi_address(spi_address), info(DATA.at(spi_address))
	{}

	SPI_TypeDef *spi() const { return reinterpret_cast<SPI_TypeDef*>(spi_address); }
};
