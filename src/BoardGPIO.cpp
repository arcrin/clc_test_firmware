#include "BoardGPIO.h"

void BoardGPIO::Init() {
    for (const auto & io : BoardGPIO::GPIODriverList) {
        io.getconstref().configure();
    }
}