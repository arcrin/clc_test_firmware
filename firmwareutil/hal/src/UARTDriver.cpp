#include "UARTDriver.h"

void UARTDriver::puts(const std::string_view s) const {
	return puts((uint8_t*)s.data(), s.size());
}

void UARTDriver::puts(const char * s, uint32_t Length) const {
	return puts((uint8_t *)s, Length);
}

void UARTDriver::puts(const uint8_t * s, uint32_t Length) const {
	assert_rts();
	while (Length--) {
		putc_raw(*s);
		s++;
	}
	deassert_rts();
}

size_t UARTDriver::ngets(uint8_t * buf, size_t Length) const {
	size_t br = 0;
	while (Length--) {
		if (!ngetc(buf[br])) {
			break;
		}
		br++;
	}
	return br;
}

void UARTDriver::flush_input() const {
	// receive all
	uint8_t c;
	while (ngetc(c));
}

void UARTDriver::assert_rts() const {
	if (current_config->RTS) {
		current_config->RTS->set_level(true);
		_delay_us(10);
	}
}

void UARTDriver::deassert_rts() const {
	if (current_config->RTS) {
		flush();
		_delay_us(10);
		current_config->RTS->set_level(false);
	}
}
