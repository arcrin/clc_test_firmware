#include "DFDriver.h"
#include "util.h"
#include <cstring>

void DFDriver::DriverInit() {

}

void DFDriver::configure() const {
	spi.configure();
}

uint64_t DFDriver::WriteVerify(uint32_t Address, const uint8_t * WData, uint32_t WLength, uint8_t Retries) const {
	uint8_t PageBuf[512];

	if (!Write(Address, WData, WLength)) {
		return 1;
	}

	while (WLength) {
		uint32_t Length = std::min(WLength, 512UL);
		uint64_t Error = 0;
		for (uint8_t t = 0; t <= Retries; t++) {
			Error = 0;

			if (!Read(Address, PageBuf, Length)) {
				Error = 2;
				continue;
			}

			if (memcmp(WData, PageBuf, Length) != 0) {
				// Find exact address
				for (uint32_t i = 0; i < 512; i++) {
					uint8_t Dif = WData[i] ^ PageBuf[i];
					if (Dif) {
						Error = ((uint64_t)(Address + i) << 32) | (WData[i] << 12) | (PageBuf[i] << 4) | 3;
						break;
					}
				}
				continue;
			}
			break;
		}
		if (Error) {
			return Error;
		}

		Address += Length;
		WData += Length;
		WLength -= Length;
	}
	return 0;
}

bool DFDriver::Write(uint32_t Address, const uint8_t * WData, uint32_t WLength) const {
	if (!WaitBusy()) {
		return false;
	}

	uint8_t ProgCommand[] = {0x02, (uint8_t)(Address >> 16), (uint8_t)(Address >> 8), (uint8_t)(Address)};
	if (!spi.TransferHalf(ProgCommand, sizeof(ProgCommand), nullptr, 0, false, false)) {
		return false;
	}

	if (!spi.TransferHalf(WData, WLength, nullptr, 0)) {
		return false;
	}
	return true;
}

bool DFDriver::Read(uint32_t Address, uint8_t * RData, uint32_t RLength) const {
	if (!WaitBusy()) {
		return false;
	}

	uint8_t Command[] = {0x0B, (uint8_t)(Address >> 16), (uint8_t)(Address >> 8), (uint8_t)(Address), 0};
	return spi.TransferHalf(Command, sizeof(Command), RData, RLength);
}

bool DFDriver::GetStatus(uint16_t &Status) const {
	uint8_t out[2];
	uint8_t Command[1] = {0xD7};
	if (!spi.TransferHalf(Command, sizeof(Command), out, 2)) {
		return false;
	}
	Status = out[0] << 8 | out[1];
	return true;
}

bool DFDriver::Busy(bool &Busy) const {
	uint16_t Status;
	if (!GetStatus(Status)) {
		return false;
	}
	Busy = (Status & 0x8080) != 0x8080;
	return true;
}

bool DFDriver::WaitBusy(uint32_t poll_ms) const {
	bool busy;
	while (1) {
		if (!Busy(busy)) {
			return false;
		}
		if (!busy) {
			return true;
		}
		if (poll_ms) {
			_delay_ms(poll_ms);
		}
	}
}

bool DFDriver::ChipErase(bool Wait) const {
	uint8_t Command[4] = {0xC7, 0x94, 0x80, 0x9A};
	if (!spi.TransferHalf(Command, sizeof(Command), nullptr, 0)) {
		return false;
	}
	if (Wait) {
		return WaitBusy(100);
	}
	return true;
}
