#include "BoardGPIO.h"
#include "string.h"

void BoardGPIO::Init() {
    GPIODriver::DriverInit();
    for (const auto & io : BoardGPIO::GPIODriverList) {
        io.getconstref().configure();
    }
}