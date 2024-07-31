#include "AFIODriver.h" 
#include <cstdint>

void AFIODriver::DriverInit() {
    #ifdef HAL_STM32F103xB
    RCC->APB2ENR = RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    #else
    #error "AFIODriver not implemented for this HAL"
    #endif
}

void AFIODriver::configure() const {
    afio()->MAPR = afio()->MAPR | (remap_value << (static_cast<uint8_t>(peripheral)));
}

void AFIODriver::reset() const {
    afio()->MAPR = afio()->MAPR & ~(0x3 << (static_cast<uint8_t>(peripheral)));
}