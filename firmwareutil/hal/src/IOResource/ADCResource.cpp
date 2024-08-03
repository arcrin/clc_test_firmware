#include "ADCResource.h"    
#include "Parser.h"
#include "BoardADC.h"

void ADCResource::Init() {
    ADCDriver::DriverInit();
    BoardADC::Init();
}

CommandHandler(ADCResource::Handler) {
    switch (Command) {
        case R: {
            uint16_t Value;
            const ADCResource * Channel;
            if (!NumArgs) {
                return SHELLERR::OK;
            }
            Channel = BoardADC::GetADCResource(Args[0]);
            if (!Channel) {
                return SHELLERR::ARGUMENT;
            }
            if (!Channel->Read(Value)) {
                return SHELLERR::EXECUTION;
            }
            Parser::DecStringFromNumber(Value, Output);
            return SHELLERR::OK;
        }
    }
    return SHELLERR::IMPLEMENTATION;
}