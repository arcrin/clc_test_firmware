#include "SPIResource.h"
#include "SPIDevice.h"
#include "Parser.h"
#include "BoardSPI.h"

void SPIResource::Init() {
	DriverInit();
	BoardSPI::Init();
}

CommandHandler(SPIResource::Handler) {
	if (NumArgs < 1) {
		return SHELLERR::ARGUMENT;
	}

	const SPIDevice *device = BoardSPI::GetSPIDevice(Args[0]);
	if (!device) {
		return SHELLERR::ARGUMENT;
	}

	Args = &Args[1];
	NumArgs--;

	switch (Command) {
		case CONF: {
			const SPIConfig * Config = BoardSPI::GetSPIConfig(Args[0]);
			if (!Config) {
				return SHELLERR::ARGUMENT;
			}
			device->spi.configure(*Config);
			return SHELLERR::OK;
		}
		case WR: {
			// <read length (non-zero implies half-duplex)> <de-assert CS at end 0/1> <assert CS each byte 0/1> <hex string>",
			if (NumArgs < 4) {
				return SHELLERR::ARGUMENT;
			}

			uint32_t RLength;
			bool Finish;
			bool AssertPerByte;

			if (!Parser::NumFromString(Args[0], RLength)) {
				return SHELLERR::ARGUMENT;
			}
			if (RLength > RESOURCE_SHELL_TEMP_ARRAY_SIZE || (RLength / 2) > RESOURCE_SHELL_OUTPUT_SIZE) {
				return SHELLERR::ARGUMENT;
			}

			if (!Parser::NumFromString(Args[1], Finish)) {
				return SHELLERR::ARGUMENT;
			}

			if (!Parser::NumFromString(Args[2], AssertPerByte)) {
				return SHELLERR::ARGUMENT;
			}

			uint32_t WLength = Args[3].size() / 2;
			if (!Parser::NumbersFromHexString(Args[3], TempArray, WLength)) {
				return SHELLERR::ARGUMENT;
			}
			if (WLength > RESOURCE_SHELL_TEMP_ARRAY_SIZE) {
				return SHELLERR::ARGUMENT;
			}
			if (!RLength && ((WLength / 2) > RESOURCE_SHELL_OUTPUT_SIZE)) {
				return SHELLERR::ARGUMENT;
			}

			if (RLength) {
				if (!device->TransferHalf(TempArray, WLength, TempArray, RLength, AssertPerByte, Finish)) {
					return SHELLERR::EXECUTION;
				}
			} else {
				if (!device->TransferFull(TempArray, WLength, TempArray, AssertPerByte, Finish)) {
					return SHELLERR::EXECUTION;
				}
				RLength = WLength;
			}
			Parser::HexStringFromNumbers(TempArray, RLength, Output);

			return SHELLERR::OK;
		}
	}
	return SHELLERR::IMPLEMENTATION;
}
