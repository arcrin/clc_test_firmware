#pragma once 
#include "IOResource.h"
#include "ADCDriver.h"

class ADCResource : public ADCDriver, public IOResource {
    using ADCDriver::ADCDriver;
    public:
        static void Init(); 
        static CommandHandler(Handler);

        CMD_MAP(
            R, "<channel name>. Read ADC channel",
        );
};