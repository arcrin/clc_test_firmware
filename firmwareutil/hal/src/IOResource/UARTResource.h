#pragma once
#include "IOResource.h"
#include "UARTDriver.h"

class UARTResource : public UARTDriver, public IOResource{
    public:
        using UARTDriver::UARTDriver;

        static void Init();
        static CommandHandler(Handler);

        CMD_MAP(
            CONF, "<uart name> <config name>. Configure the peripheral with an existing configuration",
            W,    "<uart name> <hex string>. Transmit data from the UART",
            R,    "<uart name> <# bytes>. Non-blocking receive up to `# bytes` bytes from the RX buffer",
            TEST, "<uart name> <hex byte>. Continuously transmit the hex byte until the command is interrupted",
        );
};