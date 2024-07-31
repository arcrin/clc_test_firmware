#include "AFIOResource.h"
#include "BoardAFIO.h"


void AFIOResource::Init() {
    BoardAFIO::Init();
}

CommandHandler(AFIOResource::Handler) {
    if (NumArgs < 1) {
        return SHELLERR::ARGUMENT;
    }

    Args = &Args[1];
    NumArgs--;

    const AFIODriver * afio_driver = BoardAFIO::GetAFIODriver(Args[0]);

    switch (Command) {
        case CONF: {
            afio_driver->configure();
            return SHELLERR::OK;
        }
        case RESET: {
            afio_driver->reset();
            return SHELLERR::OK;
        }
    }
    return SHELLERR::IMPLEMENTATION;
}