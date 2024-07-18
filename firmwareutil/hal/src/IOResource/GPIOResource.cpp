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
            if (!NumArgs) {
                return SHELLERR::ARGUMENT;
            }
            bool State;
            if (!Parser::NumFromString(Args[0], State)) {
                return SHELLERR::ARGUMENT;
            }
            gpio->set_level(State);
            return SHELLERR::OK;
        }
        case R: {
            bool state = gpio->get_level();
            Parser::DecStringFromNumber(state, Output);
            return SHELLERR::OK;
        }
    }
    return SHELLERR::IMPLEMENTATION;
}

// SHELLERR GPIOResource::Handler(
//     uint32_t Command, 
//     std::string_view Args[], 
//     uint32_t NumArgs, 
//     boost::static_string<2048> &Output, 
//     uint32_t &Error, 
//     bool (*InterruptOccurred)(void)
// )