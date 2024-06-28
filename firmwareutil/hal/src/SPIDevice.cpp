#include "SPIDevice.h"

void SPIDevice::configure() const {
	CS.configure();
	spi.configure(spi_config);
}

bool SPIDevice::TransferHalf(const uint8_t *WData, uint32_t WLength, uint8_t *RData, uint32_t RLength,
				  bool AssertPerByte, bool Finish) const {
	bool Result = true;
	uint8_t * RPtr = RData;
	uint32_t TotalLength = WLength + RLength;

	CS.set_level(true);
	for (uint32_t i = 0; i < TotalLength; i++) {
		uint8_t O;
		uint8_t D = (WData && (i < WLength)) ? WData[i] : 0x00;

		if (AssertPerByte) {
			CS.set_level(true);
		}
		if (!(Result = spi.TransferByte(D, O))) {
			break;
		}
		if (AssertPerByte) {
			CS.set_level(false);
			_delay_us(1);
		}
		if (i >= WLength) {
			if (RData) {
				*RPtr = O;
				RPtr++;
			}
		}
	}
	if (Finish) {
		CS.set_level(false);
	}
	return Result;
}


bool SPIDevice::TransferFull(const uint8_t *WData, uint32_t Length, uint8_t * RData,
				  bool AssertPerByte, bool Finish) const {
	bool Result = true;

	CS.set_level(true);
	for (uint32_t i = 0; i < Length; i++) {
		if (AssertPerByte) {
			CS.set_level(true);
		}
		if (!(Result = spi.TransferByte(WData[i], RData[i]))) {
			break;
		}
		if (AssertPerByte) {
			CS.set_level(false);
			_delay_us(1);
		}
	}
	if (Finish) {
		CS.set_level(false);
	}
	return Result;
}

//
//bool SPIDevice::Transfer(const uint8_t *WData, uint32_t WLength, uint8_t * RData, uint32_t RLength,
//			  bool WriteThenRead, bool AssertPerByte, bool Finish) const {
//	bool Result = true;
//	uint8_t * RPtr = RData;
//	uint32_t TotalLength = WriteThenRead ? (WLength + RLength) : WLength;
//
//	CS.set_level(true);
//	for (uint32_t i = 0; i < TotalLength; i++) {
//		uint8_t O;
//		uint8_t D = (WData && (i < WLength)) ? WData[i] : 0x00;
//
//		if (AssertPerByte) {
//			CS.set_level(true);
//		}
//		if (!(Result = spi.TransferByte(D, &O))) {
//			break;
//		}
//		if (AssertPerByte) {
//			CS.set_level(false);
//			_delay_us(1);
//		}
//		if (i >= WLength || !WriteThenRead) {
//			if (RData) {
//				*RPtr = O;
//				RPtr++;
//			}
//		}
//	}
//	if (Finish) {
//		CS.set_level(false);
//	}
//	return Result;
//}
