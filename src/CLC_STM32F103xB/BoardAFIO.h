#pragma 
#include "AFIODriver.h"
#include "IOResource.h" 

class BoardAFIO {
    public:
        static void Init();
        STATIC_VALS (
            AFIODriver,
            CAN_REMAP, (AFIODriver::PERIPHERAL_MAP::CAN, 0x03)
        )
};