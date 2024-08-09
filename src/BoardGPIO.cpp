#include "BoardGPIO.h"
#include "string.h"

void BoardGPIO::Init() {
    // NOTE: There are many overlap among the GPIO pins. Better not to auto configure all resources.
    GPIODriver::DriverInit();
    for (const auto & io : BoardGPIO::GPIODriverList) {
        io.getconstref().configure();
    }
}