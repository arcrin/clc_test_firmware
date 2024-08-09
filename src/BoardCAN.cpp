#include "BoardCAN.h"

void BoardCAN::Init() {
    for (const auto& can_driver : CANDriverList) {
        can_driver.getconstref().configure();
    }
}