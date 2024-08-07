#pragma once
#include "IOMAP.h"
#include "GPIODriver.h"

struct CANHAL {
    const uint32_t can_address;
    const GPIODriver TX;
    const GPIODriver RX;
    CAN_TypeDef *can() const { return reinterpret_cast<CAN_TypeDef*>(can_address);}
};