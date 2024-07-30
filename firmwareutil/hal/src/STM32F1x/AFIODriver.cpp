#include "AFIODriver.h" 

void AFIODriver::configure() const {
    afio()->MAPR = afio()->MAPR | (remap_value << (static_cast<uint8_t>(peripheral)));
}

void AFIODriver::reset() const {
    afio()->MAPR = afio()->MAPR & ~(0x3 << (static_cast<uint8_t>(peripheral)));
}