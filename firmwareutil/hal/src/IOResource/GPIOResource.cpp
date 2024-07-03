#include "GPIOResource.h"
#include "Parser.h"
#include "BoardGPIO.h"

void GPIOResource::Init() {
    BoardGPIO::Init();  
}

CommandHandler(GPIOResource::Handler) {
    if (NumArgs < 1) {
        return SHELLERR::ARGUMENT;
    }

    const GPIODriver *gpio = BoardGPIO::GetGPIODriver(Args[0]);
    if (!gpio) {
        return SHELLERR::ARGUMENT;
    }

    Args = &Args[1];
    NumArgs--;

    switch (Command) {
        case CONF: {
            gpio->configure();
            return SHELLERR::OK;
        }
        case W: {
            if(!NumArgs) {
                return SHELLERR::ARGUMENT;
            }

            bool state;
            if (!Parser::NumFromString(Args[0], state)) {
                return SHELLERR::ARGUMENT;
            }

            gpio->set_level(state);
            return SHELLERR::OK;
        }
        case R: {
            bool state = gpio->get_level();
            Parser::DecStringFromNumber(state, Output);
            return SHELLERR::OK;
        }
    }
    return SHELLERR::IMPLEMENTATION;
};