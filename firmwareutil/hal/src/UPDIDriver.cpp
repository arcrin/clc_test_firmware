#include "UPDIDriver.h"

void UPDIDriver::DriverInit() {
}

void UPDIDriver::configure() const {
	uart.configure(uart_config);
}

uint32_t UPDIDriver::updi_reinit() const {
	uart.configure(uart_config);
	uart.disable_rx();
	// send double break to reset UPDI layer
	uart.current_config->TX.set_level(false);
	uart.current_config->TX.set_mode(GPIODriver::MODE::OUTPUT);
	_delay_ms(30);
	uart.current_config->TX.set_level(true);
	_delay_ms(3);
	uart.current_config->TX.set_level(false);
	_delay_ms(30);
	uart.current_config->TX.set_level(true);
	_delay_ms(10);


	stcs(UPDI_CS_CTRLB, 1 << UPDI_CTRLB_CCDETDIS_BIT);
	stcs(UPDI_CS_CTRLA, (1 << UPDI_CTRLA_IBDLY_BIT));// | (1 << UPDI_CTRLA_DTD_BIT));
	if (!check()) {
		return 1;
	}
	uint32_t code = enter_progmode();
	if (code) {
		return 2 | code << 4;
	}
	return 0;
}

uint32_t UPDIDriver::enter_progmode() const {
	uint32_t error = 0;

	// TODO this initial reset supposedly isn't needed, but without it the device will read back incorrectly if it's
	//  already in prog mode. `ldcs(UPDI_ASI_KEY_STATUS)` returns 0x82.
	// Toggle reset
	if ((error = reset()) != 0) {
		return 1 | (error << 4);
	}

	if (!in_prog_mode()) {
		// Put in the key
		key(UPDI_KEY_64, UPDI_KEY_NVM, sizeof(UPDI_KEY_NVM));

		// Check key status
		uint8_t result;
		if (!ldcs(UPDI_ASI_KEY_STATUS, result) || !(result & (1 << UPDI_ASI_KEY_STATUS_NVMPROG))) {
			return 2;
		}
	}

	// Toggle reset
	if ((error = reset()) != 0) {
		return 3 | (error << 4);
	}

	// Wait for NVMPROG flag
	if (!ITERATION_TIMEOUT(in_prog_mode(), true, 10, 10000)) {
		return 4;
	}
	return 0;
}

bool UPDIDriver::check() const {
	uint8_t result;
	return ldcs(UPDI_CS_STATUSA, result) && result != 0;
}

//bool UPDIDriver::sib(boost::static_string <RESOURCE_SHELL_OUTPUT_SIZE> &Output) {
//	uint8_t cmd[3] = {UPDI_PHY_SYNC,UPDI_KEY | UPDI_KEY_SIB | UPDI_SIB_16BYTES};
//	send(cmd, 3);
//
//	uint8_t c = 0;
//	uint8_t buf[64];
//	uint8_t *ptr = buf;
//	while (1) {
//		if (!WAIT_TIMEOUT(T::uart.ngetc(c, false), true, 40, 50)) {
//			*ptr = 0;
//			break;
//		}
//		if (c == 0 || ptr >= (buf + 64)) {
//			*ptr = 0;
//			break;
//		}
//		*ptr = c;
//		ptr++;
//	}
//	return {(const char*)buf};
//}

uint32_t UPDIDriver::write_nvm_page(uint16_t address, const uint8_t buf[64]) const {
	uint32_t error = 0;
	// Wait until flash is ready
	if ((error = wait_flash_ready())) {
		return 1 | (error << 4);
	}

	// Clear the page buffer
	if (!execute_nvm_command(UPDI_V0_NVMCTRL_CTRLA_PAGE_BUFFER_CLR)) {
		return 2;
	}

	// Wait until flash is ready
	if ((error = wait_flash_ready())) {
		return 3 | (error << 4);
	}

	// Write page buffer using repeated ST16
	// first load the pointer register
	if (!set_updi_ptr(address)) {
		return 4;
	}

	// send REPEAT command.
	uint8_t rpt_cmd[] = {UPDI_PHY_SYNC, (uint8_t)(UPDI_REPEAT | UPDI_REPEAT_BYTE), 31};
	send(rpt_cmd, sizeof(rpt_cmd));

//	// TODO no way to catch errors except readback since ACKs are off
//	// turn ACKs off
	stcs(UPDI_CS_CTRLA, (uint8_t)((1 << UPDI_CTRLA_IBDLY_BIT) | (1 << UPDI_CTRLA_RSD_BIT)));

	// send first ST header
	uint8_t st16_cmd[] = {UPDI_PHY_SYNC, (uint8_t)(UPDI_ST | UPDI_PTR_INC | UPDI_DATA_16)};
	send(st16_cmd, sizeof(st16_cmd));
	send(buf, 64);

//	// turn ACKs on
	stcs(UPDI_CS_CTRLA, (uint8_t)((1 << UPDI_CTRLA_IBDLY_BIT)));

	// Commit page to NVM
	if (!execute_nvm_command(UPDI_V0_NVMCTRL_CTRLA_WRITE_PAGE)) {
		return 5;
	}

	// Wait until flash is ready
	if ((error = wait_flash_ready())) {
		return 6 | (error << 4);
	}

	return 0;
}

uint32_t UPDIDriver::chip_erase() const {
	uint32_t error = 0;
#if 0
	// Erase via CHIPERASE key

	// Put in the key
	key(UPDI_KEY_64, UPDI_KEY_CHIPERASE, sizeof(UPDI_KEY_CHIPERASE));

	// Check key status
	if (!(ldcs(UPDI_ASI_KEY_STATUS) & (1 << UPDI_ASI_KEY_STATUS_CHIPERASE))) {
		return 1;
	}

	// Toggle reset
	if ((error = reset()) != 0) {
		return 2 | (error << 4);
	}

	// Wait for LOCKSTATUS flag to go false to indicate chip erase is finished (TODO chip erase only takes 4 ms according to datasheet?)
	uint8_t stat=0;
	if (!WAIT_TIMEOUT((stat=ldcs(UPDI_ASI_SYS_STATUS)) & (1 << UPDI_ASI_SYS_STATUS_LOCKSTATUS), false, 10, 100000)) {
		return 3 | (stat << 8);
	}

	// TODO check nonexistant ERASE_FAILED bit in ASI_SYS_STATUS
//	if (ldcs(UPDI_ASI_SYS_STATUS) & (1 << UPDI_ASI_SYS_STATUS)) {
//		return 4;
//	}
	return 0;
#else
	// Erase via NVM controller

	// Wait until flash is ready
	if ((error = wait_flash_ready())) {
		return 1 | (error << 4);
	}

	// Erase
	if (!execute_nvm_command(UPDI_V0_NVMCTRL_CTRLA_CHIP_ERASE)) {
		return 2;
	}

	// Wait until flash is ready
	if ((error = wait_flash_ready())) {
		return 3 | (error << 4);
	}

	return 0;
#endif
}

bool UPDIDriver::execute_nvm_command(uint8_t command) const {
	return sts8(DEFAULT_NVMCTRL_ADDRESS + UPDI_NVMCTRL_CTRLA, command);
}

uint32_t UPDIDriver::wait_flash_ready() const {
	uint8_t status = 0;

	if(!ITERATION_TIMEOUT(
		((lds8(DEFAULT_NVMCTRL_ADDRESS + UPDI_NVMCTRL_STATUS, status), status) &
		 ((1 << UPDI_NVM_STATUS_EEPROM_BUSY) | (1 << UPDI_NVM_STATUS_FLASH_BUSY))),
		false, 1000, 1000)) {
		return 1;
	}

	if (status & (1 << UPDI_NVM_STATUS_WRITE_ERROR)) {
		return 2;
	}

	return 0;
}

uint32_t UPDIDriver::read_nvm_page(uint16_t address, uint8_t buf[64]) const {
	// read using REPEAT instruction

	// first load the pointer register
	if (!set_updi_ptr(address)) {
		return 1;
	}

	uint8_t *ptr = buf;
	uint32_t length = 64;
	while (length) {
		uint32_t chunk_size = std::min(256UL, length);

		// send REPEAT command. This allows us to skip the LD header for consecutive bytes
		uint8_t rpt_cmd[] = {UPDI_PHY_SYNC, (uint8_t)(UPDI_REPEAT | UPDI_REPEAT_BYTE), (uint8_t)(chunk_size - 1)};
		send(rpt_cmd, sizeof(rpt_cmd));

		// send first LD header
		uint8_t ld_cmd[] = {UPDI_PHY_SYNC, (uint8_t)(UPDI_LD | UPDI_PTR_INC | UPDI_DATA_8)};
		send(ld_cmd, sizeof(ld_cmd));

		// receive `chunk_size` bytes
		uint32_t num_received = receive(ptr, chunk_size);
		if (num_received != chunk_size) {
			return 2;
		}

		length -= chunk_size;
		ptr += chunk_size;
	}

	return 0;
}

bool UPDIDriver::set_updi_ptr(uint16_t address) const {
	return st16(address);
}


bool UPDIDriver::in_prog_mode() const {
	uint8_t result;
	return ldcs(UPDI_ASI_SYS_STATUS, result) && (result & (1 << UPDI_ASI_SYS_STATUS_NVMPROG));
}

uint32_t UPDIDriver::reset() const {
	// Apply reset
	uint8_t result;
	stcs(UPDI_ASI_RESET_REQ, UPDI_RESET_REQ_VALUE);
	if (!ldcs(UPDI_ASI_SYS_STATUS, result) || !(result & (1 << UPDI_ASI_SYS_STATUS_RSTSYS))) {
		return 1;
	}

	// Release reset
	stcs(UPDI_ASI_RESET_REQ, 0x00);
	// Wait for release
	if (!ITERATION_TIMEOUT((ldcs(UPDI_ASI_SYS_STATUS, result), result) & (1 << UPDI_ASI_SYS_STATUS_RSTSYS), false, 10, 10000)) {
		return 2;
	}
	return 0;
}

void UPDIDriver::key(uint8_t size, const uint8_t * key, uint8_t key_length) const {
	uint8_t cmd[] = {UPDI_PHY_SYNC, (uint8_t)(UPDI_KEY | UPDI_KEY_KEY | size)};
	send(cmd, sizeof(cmd));
	send(key, key_length);
}

bool UPDIDriver::sts8(uint16_t address, uint8_t data) const {
	uint8_t cmd[] = {UPDI_PHY_SYNC, (uint8_t)(UPDI_STS | UPDI_ADDRESS_16 | UPDI_DATA_8),
					 (uint8_t)(address & 0xFF), (uint8_t)((address >> 8) & 0xFF)};

	send(cmd, sizeof(cmd));
	if (!wait_ack()) {
		return false;
	}

	send(&data, 1);
	return wait_ack();
}

bool UPDIDriver::st16(uint16_t data) const {
	uint8_t cmd[] = {UPDI_PHY_SYNC, (uint8_t)(UPDI_ST | UPDI_PTR_ADDRESS | UPDI_DATA_16),
					 (uint8_t)(data & 0xFF), (uint8_t)((data >> 8) & 0xFF)};
	send(cmd, sizeof(cmd));
	return wait_ack();
}

bool UPDIDriver::lds8(uint16_t address, uint8_t &result) const {
	uint8_t cmd[] = {UPDI_PHY_SYNC, (uint8_t)(UPDI_LDS | UPDI_ADDRESS_16 | UPDI_DATA_8),
					 (uint8_t)(address & 0xFF), (uint8_t)((address >> 8) & 0xFF)};
	send(cmd, sizeof(cmd));
	return receive(&result, 1) == 1;
}

bool UPDIDriver::ldcs(uint8_t address, uint8_t &result) const {
	uint8_t cmd[2] = {UPDI_PHY_SYNC, (uint8_t)(UPDI_LDCS | (address & 0x0F))};
	send(cmd, sizeof(cmd));
	return receive(&result, 1) == 1;
}

void UPDIDriver::stcs(uint8_t address, uint8_t value) const {
	uint8_t cmd[3] = {UPDI_PHY_SYNC, (uint8_t)(UPDI_STCS | (address & 0x0F)), value};
	send(cmd, sizeof(cmd));
}

void UPDIDriver::send(const uint8_t *data, uint32_t length) const {
	uart.disable_rx();
	uart.current_config->TX.configure();
	while (length--) {
		uart.putc(*data);
		uart.flush();
		data++;
		_delay_us(10);
		uart.flush_input();
	}
	uart.current_config->TX.set_mode(GPIODriver::MODE::INPUT);
	uart.enable_rx();
}

bool UPDIDriver::wait_ack() const {
	uint8_t ack;
	return receive(&ack, 1) == 1 && ack == UPDI_PHY_ACK;
}

size_t UPDIDriver::receive(uint8_t *buf, uint32_t length) const {
	uint8_t c;
	uint8_t *ptr = buf;
	while (length) {
		if (ITERATION_TIMEOUT(uart.ngetc(c), true, 100, 50)) {
			if (buf) { // TODO may remove this
				*ptr = c;
			}
			ptr++;
			length--;
		} else {
			break;
		}
	}
	return ptr - buf;
}
