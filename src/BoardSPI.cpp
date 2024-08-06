#include "BoardSPI.h"

void BoardSPI::Init() {
    for (const auto & spi_device : BoardSPI::SPIDeviceList) {
        spi_device.getconst().configure();
    }
}