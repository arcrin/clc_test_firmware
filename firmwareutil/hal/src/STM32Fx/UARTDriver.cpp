#include "IOMAP.h"
#include "UARTDriver.h"
#include "UARTConfig.h"

void UARTDriver::DriverInit() {
}

void UARTDriver::configure(UARTConfig const & config) const {
	// unconfigure the current pins
	if (current_config && (&current_config->TX != &config.TX)) current_config->TX.unconfigure();
	if (current_config && (&current_config->RX != &config.RX)) current_config->RX.unconfigure();
	if (current_config && current_config->RTS && (current_config->RTS != config.RTS))

	current_config = &config;	

	hal.info.ENR_REG() |= 1 << hal.info.ENR_POS;

	#ifdef HAL_STM32F103xB
		hal.MAPR_REG() = (hal.MAPR_REG() & ~(hal.info.REMAP_MASK)) | (hal.remap << hal.info.REMAP_POS);
	#endif

	uint32_t APBx_clock = F_CPU / hal.info.Divider;

	uint32_t brr_x16 = APBx_clock / current_config->baud;
	uint32_t mantissa = brr_x16 / 16;
	uint32_t fraction = brr_x16 - (mantissa * 16);

	current_config->TX.configure();
	current_config->RX.configure();
	if (current_config->RTS) current_config->RTS->configure();

	hal.uart()->BRR = (mantissa << 4) | fraction;

	// Use RX interrupt
	hal.uart()->CR1 = hal.uart()->CR1 | USART_CR1_RXNEIE;
	
	NVIC_SetPriority(ToIRQ(hal.info.IRQIndex), 0);
	NVIC_EnableIRQ(ToIRQ(hal.info.IRQIndex));

	hal.uart()->CR1 = hal.uart()->CR1 | USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

void UARTDriver::flush() const {
	while(!(hal.uart()->SR & USART_SR_TC));
}

void UARTDriver::putc(char c) const {
	return putc((uint8_t)c);
}

void UARTDriver::putc(uint8_t c) const {
	while(!(hal.uart()->SR & USART_SR_TXE));

	assert_rts();
	putc_raw(c);
	deassert_rts();
}

void UARTDriver::putc_raw(uint8_t c) const {
	while(!hal.uart()->SR & USART_SR_TXE);
	hal.uart()->DR = c;	
}

// non-blocking get character. Will return true if character received
bool UARTDriver::ngetc(uint8_t &c) const {
	return Buffer.Get(&c) == 0;
}

void UARTDriver::IRQHandler() const {
	while (hal.uart()->SR & (USART_SR_ORE | USART_SR_RXNE)) {
		uint8_t c = hal.uart()->DR;
		Buffer.Put(c);
	}
}