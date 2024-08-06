#pragma once
#include "IOResource.h"
#include "IOMAP.h"
#include "GPIODriver.h"
#include "SPIConfig.h"
#include "SPIResource.h"
#include "SPIDevice.h"

class BoardSPI {
    public:
        static void Init();

        STATIC_VALS(
            SPIResource,
            SPI_EEPROM, ({SPI2_BASE})
        );

        STATIC_VALS(
            SPIConfig,
            EEPROM, (1000000,
                     {{GPIOB_BASE, 13}, GPIODriver::MODE::OUTPUT, 0, false, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::HIGH},
                     {{GPIOB_BASE, 14}, GPIODriver::MODE::INPUT, 0, false, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::HIGH},
                     {{GPIOB_BASE, 15}, GPIODriver::MODE::OUTPUT, 0, false, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::HIGH},
                     false, 
                     false), // clock
        );

        static inline SPIDevice EEPROM_DEVICE {
                                                GetSPIResource(SPIResource_t::SPI_EEPROM), 
                                                GetSPIConfig(SPIConfig_t::EEPROM), 
                                                {{GPIOB_BASE, 12}, GPIODriver::MODE::ALTERNATE, 0, false, false, GPIODriver::TYPE::OPEN_DRAIN, GPIODriver::SPEED::HIGH}
                                                };
        STATIC_REFS(
            SPIDevice, 
            EEPROM, EEPROM_DEVICE
        )
};