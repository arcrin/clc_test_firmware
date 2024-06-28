#pragma once
#include "UARTDriver.h"

// modified from pyupdi: https://github.com/mraardvark/pyupdi
/// This driver is processor-independent. It only relies on other drivers: UART
/// An instance of this class represents a UPDI bus
class UPDIDriver {
public:
	/// Create a UPDI bus
	/// \param uart UART peripheral that is used by the UPDI bus
	/// \param uart_config UART configuration to use for UPDI
	constexpr UPDIDriver(const UARTDriver & uart, const UARTConfig & uart_config):
	uart(uart), uart_config(uart_config)
	{}
	UPDIDriver() = delete;
	~UPDIDriver() = default;

	/// Common initialization of the UPDI system
	static void DriverInit();

	/// (Re-)configure the UPDI UART
	void configure() const;

	/// (Re)-configure the UART, reset the UPDI bus, and enter programming mode
	/// \return 0 on success
	uint32_t updi_reinit() const;
	uint32_t enter_progmode() const;
	bool check() const;
//	bool sib(boost::static_string <RESOURCE_SHELL_OUTPUT_SIZE> &Output) const;
	uint32_t write_nvm_page(uint16_t address, const uint8_t buf[64]) const;
	uint32_t chip_erase() const;
	bool execute_nvm_command(uint8_t command) const;
	uint32_t wait_flash_ready() const;
	uint32_t read_nvm_page(uint16_t address, uint8_t buf[64]) const;
	bool set_updi_ptr(uint16_t address) const;
	bool in_prog_mode() const;
	uint32_t reset() const;
	void key(uint8_t size, const uint8_t * key, uint8_t key_length) const;
	bool sts8(uint16_t address, uint8_t data) const;
	bool st16(uint16_t data) const;
	bool lds8(uint16_t address, uint8_t &result) const;
	bool ldcs(uint8_t address, uint8_t &result) const;
	void stcs(uint8_t address, uint8_t value) const;
	void send(const uint8_t *data, uint32_t length) const;
	bool wait_ack() const;
	size_t receive(uint8_t *buf, uint32_t length) const;

	// UPDI commands and control definitions
	static constexpr uint8_t UPDI_BREAK = 0x00;

	static constexpr uint8_t UPDI_LDS = 0x00;
	static constexpr uint8_t UPDI_STS = 0x40;
	static constexpr uint8_t UPDI_LD = 0x20;
	static constexpr uint8_t UPDI_ST = 0x60;
	static constexpr uint8_t UPDI_LDCS = 0x80;
	static constexpr uint8_t UPDI_STCS = 0xC0;
	static constexpr uint8_t UPDI_REPEAT = 0xA0;
	static constexpr uint8_t UPDI_KEY = 0xE0;

	static constexpr uint8_t UPDI_PTR = 0x00;
	static constexpr uint8_t UPDI_PTR_INC = 0x04;
	static constexpr uint8_t UPDI_PTR_ADDRESS = 0x08;
//
	static constexpr uint8_t UPDI_ADDRESS_8 = 0x00;
	static constexpr uint8_t UPDI_ADDRESS_16 = 0x04;
	static constexpr uint8_t UPDI_ADDRESS_24 = 0x08;

	static constexpr uint8_t UPDI_DATA_8 = 0x00;
	static constexpr uint8_t UPDI_DATA_16 = 0x01;
	static constexpr uint8_t UPDI_DATA_24 = 0x02;

	static constexpr uint8_t UPDI_KEY_SIB = 0x04;
	static constexpr uint8_t UPDI_KEY_KEY = 0x00;

	static constexpr uint8_t UPDI_KEY_64 = 0x00;
	static constexpr uint8_t UPDI_KEY_128 = 0x01;

	static constexpr uint8_t UPDI_SIB_8BYTES = UPDI_KEY_64;
	static constexpr uint8_t UPDI_SIB_16BYTES = UPDI_KEY_128;

	static constexpr uint8_t UPDI_REPEAT_BYTE = 0x00;
	static constexpr uint8_t UPDI_REPEAT_WORD = 0x01;

	static constexpr uint8_t UPDI_PHY_SYNC = 0x55;
	static constexpr uint8_t UPDI_PHY_ACK = 0x40;

//	static constexpr uint8_t UPDI_MAX_REPEAT_SIZE = (0xFF+1);    // Repeat counter of 1-byte, with off-by-one counting

	// CS and ASI Register Address map
	static constexpr uint8_t UPDI_CS_STATUSA = 0x00;
	static constexpr uint8_t UPDI_CS_STATUSB = 0x01;
	static constexpr uint8_t UPDI_CS_CTRLA = 0x02;
	static constexpr uint8_t UPDI_CS_CTRLB = 0x03;
	static constexpr uint8_t UPDI_ASI_KEY_STATUS = 0x07;
	static constexpr uint8_t UPDI_ASI_RESET_REQ = 0x08;
	static constexpr uint8_t UPDI_ASI_CTRLA = 0x09;
	static constexpr uint8_t UPDI_ASI_SYS_CTRLA = 0x0A;
	static constexpr uint8_t UPDI_ASI_SYS_STATUS = 0x0B;
	static constexpr uint8_t UPDI_ASI_CRC_STATUS = 0x0C;

	static constexpr uint8_t UPDI_CTRLA_IBDLY_BIT = 7;
	static constexpr uint8_t UPDI_CTRLA_DTD_BIT = 4;
	static constexpr uint8_t UPDI_CTRLA_RSD_BIT = 3;
	static constexpr uint8_t UPDI_CTRLB_CCDETDIS_BIT = 3;
	static constexpr uint8_t UPDI_CTRLB_UPDIDIS_BIT = 2;

	static constexpr uint8_t UPDI_KEY_NVM[8] = {' ', 'g', 'o', 'r', 'P', 'M', 'V', 'N'};
	static constexpr uint8_t UPDI_KEY_CHIPERASE[8] = {'e', 's', 'a', 'r', 'E', 'M','V', 'N'};

	static constexpr uint8_t UPDI_ASI_STATUSA_REVID = 4;
	static constexpr uint8_t UPDI_ASI_STATUSB_PESIG = 0;

	static constexpr uint8_t UPDI_ASI_KEY_STATUS_CHIPERASE = 3;
	static constexpr uint8_t UPDI_ASI_KEY_STATUS_NVMPROG = 4;
	static constexpr uint8_t UPDI_ASI_KEY_STATUS_UROWWRITE = 5;

	static constexpr uint8_t UPDI_ASI_SYS_STATUS_RSTSYS = 5;
	static constexpr uint8_t UPDI_ASI_SYS_STATUS_INSLEEP = 4;
	static constexpr uint8_t UPDI_ASI_SYS_STATUS_NVMPROG = 3;
	static constexpr uint8_t UPDI_ASI_SYS_STATUS_UROWPROG = 2;
	static constexpr uint8_t UPDI_ASI_SYS_STATUS_LOCKSTATUS = 0;

	static constexpr uint8_t UPDI_RESET_REQ_VALUE = 0x59;

	// FLASH CONTROLLER;
	static constexpr uint8_t UPDI_NVMCTRL_CTRLA = 0x00;
	static constexpr uint8_t UPDI_NVMCTRL_CTRLB = 0x01;
	static constexpr uint8_t UPDI_NVMCTRL_STATUS = 0x02;
	static constexpr uint8_t UPDI_NVMCTRL_INTCTRL = 0x03;
	static constexpr uint8_t UPDI_NVMCTRL_INTFLAGS = 0x04;
	static constexpr uint8_t UPDI_NVMCTRL_DATAL = 0x06;
	static constexpr uint8_t UPDI_NVMCTRL_DATAH = 0x07;
	static constexpr uint8_t UPDI_NVMCTRL_ADDRL = 0x08;
	static constexpr uint8_t UPDI_NVMCTRL_ADDRH = 0x09;

	// NVMCTRL v0 CTRLA;
	static constexpr uint8_t UPDI_V0_NVMCTRL_CTRLA_NOP = 0x00;
	static constexpr uint8_t UPDI_V0_NVMCTRL_CTRLA_WRITE_PAGE = 0x01;
	static constexpr uint8_t UPDI_V0_NVMCTRL_CTRLA_ERASE_PAGE = 0x02;
	static constexpr uint8_t UPDI_V0_NVMCTRL_CTRLA_ERASE_WRITE_PAGE = 0x03;
	static constexpr uint8_t UPDI_V0_NVMCTRL_CTRLA_PAGE_BUFFER_CLR = 0x04;
	static constexpr uint8_t UPDI_V0_NVMCTRL_CTRLA_CHIP_ERASE = 0x05;
	static constexpr uint8_t UPDI_V0_NVMCTRL_CTRLA_ERASE_EEPROM = 0x06;
	static constexpr uint8_t UPDI_V0_NVMCTRL_CTRLA_WRITE_FUSE = 0x07;

	// NVMCTRL v1 CTRLA;
	static constexpr uint8_t UPDI_V1_NVMCTRL_CTRLA_NOCMD = 0x00;
	static constexpr uint8_t UPDI_V1_NVMCTRL_CTRLA_FLASH_WRITE = 0x02;
	static constexpr uint8_t UPDI_V1_NVMCTRL_CTRLA_EEPROM_ERASE_WRITE = 0x13;
	static constexpr uint8_t UPDI_V1_NVMCTRL_CTRLA_CHIP_ERASE = 0x20;

	static constexpr uint8_t UPDI_NVM_STATUS_WRITE_ERROR = 2;
	static constexpr uint8_t UPDI_NVM_STATUS_EEPROM_BUSY = 1;
	static constexpr uint8_t UPDI_NVM_STATUS_FLASH_BUSY = 0;


	static constexpr uint16_t DEFAULT_SYSCFG_ADDRESS = 0x0F00;
	static constexpr uint16_t DEFAULT_NVMCTRL_ADDRESS = 0x1000;
	static constexpr uint16_t DEFAULT_SIGROW_ADDRESS = 0x1100;
	static constexpr uint16_t DEFAULT_FUSES_ADDRESS = 0x1280;
	static constexpr uint16_t DEFAULT_USERROW_ADDRESS = 0x1300;

private:
	/// UART peripheral that is used by the UPDI bus. This must be defined in the application
	const UARTDriver & uart;
	/// UART configuration to use for UPDI. This must be defined in the application
	const UARTConfig & uart_config;
};
