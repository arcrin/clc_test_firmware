#include "DFProg.h"

void DFProg::CMD_MASS_ERASE() {
	bool busy;

	if (LocalParam.Status == _MEMPROG_STATUS_START) {
		df.ChipErase(false);
		return;
	}

	if (!df.Busy(busy)) {
		LocalParam.Code = 1;
		return;
	}

	if (!busy) {
		LocalParam.Status = MEMPROG_STATUS_OK;
		return;
	}
}

void DFProg::CMD_PROG_VERIFY() {
	static uint32_t crc;
	static bool Last = false;
	static constexpr const uint32_t width = 512;

	if (LocalParam.Status == _MEMPROG_STATUS_START) {
		crc = 0;
		Last = false;
		return;
	}

	const uint8_t *RXPtr;
	uint32_t DataAddress, RXPtrLength;
	uint64_t Error;

	GetRXStream(RXPtr, DataAddress, RXPtrLength);

	if (DataAddress & (width - 1)) {
		// Unaligned write. Make sure alignment option is correct in 'memprog submit'
		LocalParam.Code = 1;
		LocalParam.Status = MEMPROG_STATUS_ERR_PARAM;
		return;
	}

	if (RXPtrLength < width) {
		// Not enough data in buffer. Make sure alignment option is correct in 'memprog submit'
		LocalParam.Code = 2;
		LocalParam.Status = MEMPROG_STATUS_ERR_PARAM;
		return;
	}

	if ((Error = df.WriteVerify(DataAddress, RXPtr, width)) != 0) {
		LocalParam.Code = 1;
		LocalParam.P1 = Error >> 32;
		LocalParam.P2 = Error;
		return;
	}
	crc = CalculateCRC32(RXPtr, width, crc);

	if (ReadStream(width, Last)) {
		return;
	}

	if (Last) {
		LocalParam.P1 = crc;
		LocalParam.Status = MEMPROG_STATUS_OK;
	}
}


void DFProg::CMD_READ() {
	bool Last = false;
	uint8_t *TXPtr;
	uint32_t TXPtrLength;
	uint32_t DataAddress;

	if (LocalParam.Status == _MEMPROG_STATUS_START) {
		return;
	}

	if (ConvertStream(TXPtr, DataAddress, TXPtrLength, Last)) {
		return;
	}

	if (!TXPtr) {
		return;
	}

	// Acquired the buffer. Read the dataflash and write to buffer
	if (!df.Read(DataAddress, TXPtr, TXPtrLength)) {
		LocalParam.Code = 1;
		return;
	}

	if (WriteStream(TXPtrLength, Last)) {
		return;
	}

	if (Last) {
		LocalParam.Status = MEMPROG_STATUS_OK;
	}
}
