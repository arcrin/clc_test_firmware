#pragma once
#include "IOResource.h"
#include "SPIDriver.h"

class SPIResource : public SPIDriver, public IOResource {
    public:
        using SPIDriver::SPIDriver;

        static void Init(); 
        static CommandHandler(Handler);

        CMD_MAP(
		CONF, "<device name> <config name>. Configure the device with an existing configuration",
		WR,   "<device name> <read length> <finish> <assert per byte> <hex string>. Transfer data with the device."
			  "\n - `read length`: Non-zero implies two half duplex transfers, a write followed by a read."
			  "\n - `finish`: 1 to de-assert CS at the end of the transfer. 0 to leave it asserted."
			  "\n - `assert per byte`: 1 to briefly de-assert CS after each byte. 0 to keep CS asserted for the entire transfer.",
        );  
};