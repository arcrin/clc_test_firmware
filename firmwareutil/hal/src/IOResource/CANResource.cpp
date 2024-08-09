#include "CANDriver.h"
#include "CANResource.h"
#include "BoardCAN.h"
#include "Parser.h"

void CANResource::Init() {
	CANDriver::DriverInit();
	BoardCAN::Init();
}

CommandHandler(CANResource::Handler) {
	if (NumArgs < 1) {
		return SHELLERR::ARGUMENT;
	}

	const CANDriver * Driver = BoardCAN::GetCANDriver(Args[0]);
	if (!Driver) {
		return SHELLERR::ARGUMENT;
	}
	Args = &Args[1];
	NumArgs--;

	switch (Command) {
		case ON: {
			Driver->Enable();
			return SHELLERR::OK;
		}
		case OFF: {
			Driver->Disable();
			return SHELLERR::OK;
		}
		case W:
		case TEST: {
			// id hex_string
			if (NumArgs < 2) {
				return SHELLERR::ARGUMENT;
			}

			uint8_t ID;
			uint8_t Length = Args[1].size() / 2;
			uint8_t Data[8];

			if (Length > 8) {
				return SHELLERR::ARGUMENT;
			}

			if (!Parser::NumFromString(Args[0], ID)) {
				return SHELLERR::ARGUMENT;
			}

			if (!Parser::NumbersFromHexString(Args[1], Data, Length)) {
				return SHELLERR::ARGUMENT;
			}

			while (1) {
				Driver->Write(Data, Length, ID);
				if (Command == W || InterruptOccurred()) {
					break;
				}
			}
			return SHELLERR::OK;
		}
		case R: {
			uint8_t Data[8];
			uint8_t Length;
			uint32_t ID;
			uint16_t Timestamp;
			bool Remote;

			if (Driver->Read(Data, Length, ID, Timestamp, Remote)) {
				Parser::DecStringFromNumber(ID, Output);
				Output += ' ';
				Parser::DecStringFromNumber(Timestamp, Output);
				Output += ' ';
				Parser::HexStringFromNumbers(Data, Length, Output);
			}
			return SHELLERR::OK;
		}
		case FLUSH: {
			Driver->Flush();
			return SHELLERR::OK;
		}
	}
	return SHELLERR::IMPLEMENTATION;
}
