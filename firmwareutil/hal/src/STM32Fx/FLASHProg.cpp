#include "FLASHProg.h"
#include "IOMAP.h"

void FLASHProg::Init() {
	// DB1M should be 0 TODO

	// Unlock CR register
	FLASH->KEYR = 0x45670123;
	FLASH->KEYR = 0xCDEF89AB;

	// Set PSIZE to 10 (32 bit)
	#ifndef HAL_STM32F103xB
	FLASH->CR = (FLASH->CR & ~FLASH_CR_PSIZE) | FLASH_CR_PSIZE_1;
	#endif

//	// Disable caches
//	FLASH->ACR &= ~(FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_PRFTEN);
//	// Reset caches
//	FLASH->ACR &= FLASH_ACR_DCRST | FLASH_ACR_ICRST;
}

void FLASHProg::ROM_READ_to_ROM_PE() {

}
void FLASHProg::ROM_READ_to_DATA_PE() {

}
void FLASHProg::PE_to_ROM_READ() {

}


static bool Busy() {
	return FLASH->SR & FLASH_SR_BSY;
}

//static bool WaitBusy(uint32_t timeout_us) {
//	return WAIT_TIMEOUT(Busy(), 0, timeout_us);
//}

static uint32_t GetError() {
	uint32_t Code = 0;
	#ifdef HAL_STM32F103xB
	if (FLASH->SR & FLASH_SR_PGERR) {
		Code = 1;
	} else if (FLASH->SR & FLASH_SR_WRPRTERR) {
		Code = 2;
	}
	FLASH->SR |= FLASH_SR_PGERR | FLASH_SR_WRPRTERR;
	#else
	if (FLASH->SR & (1 << 8)) {
		Code = 1;
	} else if (FLASH->SR & FLASH_SR_PGSERR) {
		Code = 2;
	} else if (FLASH->SR & FLASH_SR_PGPERR) {
		Code = 3;
	} else if (FLASH->SR & FLASH_SR_PGAERR) {
		Code = 4;
	} else if (FLASH->SR & FLASH_SR_WRPERR) {
		Code = 5;
	} else if (FLASH->SR & (1 << 1)) {
		Code = 6;
	}
	FLASH->SR |= (1 << 8) | FLASH_SR_PGSERR | FLASH_SR_PGPERR | FLASH_SR_PGAERR | FLASH_SR_WRPERR | (1 << 1);
	#endif
	return Code;
}


void FLASHProg::CMD_MASS_ERASE() {
	if (LocalParam.Status == _MEMPROG_STATUS_START) {
		FLASH->CR = (FLASH->CR & ~FLASH_CR_PSIZE) | FLASH_CR_PSIZE_1;
#ifdef HAL_STM32F427
		FLASH->CR |= FLASH_CR_MER | FLASH_CR_MER2;
#elif defined(HAL_STM32F205)
		FLASH->CR |= FLASH_CR_MER;
#else
#error
#endif
		FLASH->CR |= FLASH_CR_STRT;
		return;
	}

	if (Busy()) {
		return;
	}

	LocalParam.Code = GetError();
#ifdef HAL_STM32F427
	FLASH->CR &= ~(FLASH_CR_MER | FLASH_CR_MER2);
#elif defined(HAL_STM32F205)
	FLASH->CR &= ~(FLASH_CR_MER);
#else
#error
#endif

	if (LocalParam.Code) {
		LocalParam.Status = MEMPROG_STATUS_ERR_OTHER;
	} else {
		LocalParam.Status = MEMPROG_STATUS_OK;
	}
}

uint32_t FLASHProg::GetAlignment(uint32_t Address) {
	return 4;
}

uint32_t FLASHProg::PGM(uint32_t Destination, const uint8_t * Source) {
	FLASH->CR = (FLASH->CR & ~FLASH_CR_PSIZE) | FLASH_CR_PSIZE_1;
	// Length need not be aligned. Unaligned bytes will be programmed individually

	FLASH->CR |= FLASH_CR_PG;
	*((uint32_t *) Destination) = *((uint32_t *) Source);

	while (Busy());

	uint32_t err = GetError();

	FLASH->CR &= ~(FLASH_CR_PG);
	return err;
}
