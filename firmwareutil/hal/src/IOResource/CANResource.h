#pragma once
#include "IOResource.h"

class CANResource : public IOResource {
public:
	constexpr CANResource() = delete;
	constexpr ~CANResource() = delete;

	static void Init();
	static CommandHandler(Handler);

	CMD_MAP(
		ON,     "<can name>. Enable CAN peripheral",
		OFF,    "<can name>. Disable CAN peripheral",
		W,      "<can name> <id> <hex string>. Transmit data with the given ID",
		R,      "<can name>. Read a packet, if available",
		FLUSH,  "<can name>. Flush the RX and TX buffer",
		TEST,   "<can name> <id> <hex string>. Continuously transmit the packet until the command is interrupted",
	);
};
