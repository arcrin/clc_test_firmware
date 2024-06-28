#include "FLASHProg.h"
#include "SYSTICK.h"

void FLASHProg::CMD_PROG_VERIFY() {
	static uint32_t CRC = 0;
	const uint8_t *RXPtr;
	uint32_t DataAddress, RXPtrLength;
	bool Last = false;

	if (LocalParam.Status == _MEMPROG_STATUS_START) {
		CRC = 0;
		ROM_READ_to_ROM_PE();
		return;
	}

	GetRXStream(RXPtr, DataAddress, RXPtrLength);
	if (!RXPtr) {
		return;
	}

	uint32_t width = GetAlignment(DataAddress);

	if (DataAddress & (width - 1)) {
		// Unaligned write. Make sure alignment option is correct in 'memprog submit'
		PE_to_ROM_READ();
		LocalParam.Code = 1;
		LocalParam.Status = MEMPROG_STATUS_ERR_PARAM;
		return;
	}

	if (RXPtrLength < width) {
		// Not enough data in buffer. Make sure alignment option is correct in 'memprog submit'
		PE_to_ROM_READ();
		LocalParam.Code = 2;
		LocalParam.Status = MEMPROG_STATUS_ERR_PARAM;
		return;
	}

	if ((LocalParam.Code = PGM(DataAddress, RXPtr))) {
		PE_to_ROM_READ();
		return;
	}

	if (ReadStream(width, Last)) {
		PE_to_ROM_READ();
		return;
	}

	PE_to_ROM_READ();
	// calculate the CRC
	CRC = CalculateCRC32((const uint8_t*)DataAddress, width, CRC);

//	DBGH4((uint32_t)CRCPtr);
//	DBGH4(CRCLength);
//	DBGH4(CRC);
//	DBGEND();

	if (Last) {
		LocalParam.P1 = CRC;
		LocalParam.Status = MEMPROG_STATUS_OK;
	} else {
		ROM_READ_to_ROM_PE();
	}
}

//void FLASHProg::CMD_CRC() {
//	static uint32_t CRC = 0;
//	const uint8_t *RXPtr;
//	uint32_t DataAddress, RXPtrLength;
//	bool Last = false;
//
//	if (LocalParam.Status == _MEMPROG_STATUS_START) {
//		CRC = 0;
//		ROM_READ_to_ROM_PE();
//		return;
//	}
//
//	GetRXStream(RXPtr, DataAddress, RXPtrLength);
//
//	if (!RXPtr) {
//		return;
//	}
//
//	// calculate the CRC
//	CRC = CalculateCRC32((const uint8_t*)DataAddress, RXPtrLength, CRC);
//
//	// TODO could ConvertStream and send back the CRC for each buffer for debugging
//	if (ReadStream(RXPtrLength, Last)) {
//		return;
//	}
//
////	DBGH4((uint32_t)CRCPtr);
////	DBGH4(CRCLength);
////	DBGH4(CRC);
////	DBGEND();
//
//	if (Last) {
//		LocalParam.P1 = CRC;
//		LocalParam.Status = MEMPROG_STATUS_OK;
//	}
//}
