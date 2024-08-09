#pragma once
#include "IOResource.h" 
#include "GPIODriver.h"
#include "CANDriver.h"  

class BoardCAN {
    public:
        static void Init();

        STATIC_VALS(
            CANDriver,
            CAN, (
                    {
                        CAN1_BASE,
                        {{GPIOA_BASE, 11}, GPIODriver::MODE::INPUT, 0, false, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::VERY_HIGH},
                        {{GPIOA_BASE, 12}, GPIODriver::MODE::ALTERNATE, 0, false, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::VERY_HIGH}
                    }
                )
        )
};
