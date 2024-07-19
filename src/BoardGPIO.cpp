#include "BoardGPIO.h"

void BoardGPIO::Init() {
    GPIODriver::DriverInit();
    for (const auto & io : BoardGPIO::GPIODriverList) {
        io.getconstref().configure();
    }
}