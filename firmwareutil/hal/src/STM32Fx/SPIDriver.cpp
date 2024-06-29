#include "IOMAP.h"
#include "SPIDriver.h"

void SPIDriver::DriverInit() {
}

void SPIDriver::configure(SPIConfig const & config) const {
	hal.info.ENR_REG() |= 1 << hal.info.ENR_POS;

	if (&config == current_config) {
		return;
	}

	// unconfigure the current pins
	if (current_config) {
		current_config->MOSI.unconfigure();
		current_config->MISO.unconfigure();
		current_config->SCK.unconfigure();
	}

	current_config = &config;
	current_config->MOSI.configure();
	current_config->MISO.configure();
	current_config->SCK.configure();
	// TODO everything above this could be moved out of HAL
	// FIXME is this right for APB1 periphs?
	// APB2 = F_CPU / 2
	// SPI clock with lowest divider = APB2 / 2 = F_CPU / 4
	uint32_t frequency = F_CPU / 4;
	uint8_t divider = 0;
	while (frequency > current_config->bitrate && divider < 7) {
		frequency /= 2;
		divider++;
	}

	hal.spi()->CR1 &= ~SPI_CR1_SPE;

	// 4 MHz
	hal.spi()->CR1 = SPI_CR1_MSTR
			| (divider << SPI_CR1_BR_Pos)
			| (current_config->cpha ? SPI_CR1_CPHA : 0)
			| (current_config->cpol ? SPI_CR1_CPOL : 0)
			;
	hal.spi()->CR2 = SPI_CR2_SSOE;
	hal.spi()->CR1 |= SPI_CR1_SPE;
}

bool SPIDriver::TransferByte(uint8_t WData, uint8_t & RData) const {
	// wait for space in TX buffer
	while (!(hal.spi()->SR & SPI_SR_TXE));
	hal.spi()->DR = WData;
	// wait for space in TX buffer to ensure it sent out
	while (!(hal.spi()->SR & SPI_SR_TXE));
	// wait until not busy
	while ((hal.spi()->SR & SPI_SR_BSY));
	// TODO RXNE bit asserts for a little bit, then deasserts??
	// wait until RX buffer is not empty
	// while (!(hal.spi()->SR & SPI_SR_RXNE));
	RData = hal.spi()->DR;
	return true;
}
