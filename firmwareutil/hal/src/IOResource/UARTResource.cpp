#include "UARTResource.h"
#include "Parser.h"
#include "BoardUART.h"

void UARTResource::Init() {
    DriverInit();
    BoardUART::Init();
}

CommandHandler(UARTResource::Handler) {
    if (NumArgs < 1) {
        return SHELLERR::ARGUMENT;
    }
    const UARTResource * Resource = BoardUART::GetUARTResource(Args[0]);
    if (!Resource) {
        return SHELLERR::ARGUMENT;
    }
    Args = &Args[1];
    NumArgs--;

    switch (Command) {
        case CONF: {
            const UARTConfig * Config;
            if (!NumArgs) {
                return SHELLERR::OK;
            }
            Config = BoardUART::GetUARTConfig(Args[0]);
            if (!Config) {
                return SHELLERR::ARGUMENT;
            }
            Resource->configure(*Config);
            return SHELLERR::OK;
        }
        case W: {
            uint32_t WLength;
            if (!NumArgs) {
                return SHELLERR::OK;
            }
            WLength = Args[0].size() / 2;   
            if (!Parser::NumbersFromHexString(Args[0], TempArray, WLength)){
                return SHELLERR::ARGUMENT;
            }
            Resource->puts(TempArray, WLength);
            return SHELLERR::OK;    
        }
        case R: {
            uint32_t RLength;
            if (!NumArgs) {
                return SHELLERR::OK;
            }
            if (!Parser::NumFromString(Args[0], RLength)) {
                return SHELLERR::ARGUMENT;
            }
            if (RLength > RESOURCE_SHELL_TEMP_ARRAY_SIZE) {
                return SHELLERR::ARGUMENT;
            }

            RLength = Resource->ngets(TempArray, RLength);
            Parser::HexStringFromNumbers(TempArray, RLength, Output);
            return SHELLERR::OK;
        }
        case TEST: {
            uint8_t Data;
            if (!NumArgs) {
                return SHELLERR::OK;
            }
            if (!Parser::NumbersFromHexString(Args[0], &Data, 1)) {
                return SHELLERR::ARGUMENT;
            }
            memset(TempArray, Data, sizeof(TempArray));
            while(!InterruptOccurred()) {
                Resource->puts(TempArray, sizeof(TempArray));   
            }
            return SHELLERR::OK;
        }
    }
    return SHELLERR::IMPLEMENTATION;
}