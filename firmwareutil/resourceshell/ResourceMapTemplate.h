#pragma once
#include "ResourceEntry.h"

// Include the IOResource class headers
//#include "UARTResource.h"
//#include "I2CResource.h"

class ResourceMap {
public:
	ResourceMap() = default;
	~ResourceMap() = default;

protected:
	// Specify the IOResource classes inside the macro
	RESOURCE_MAP(
//		UARTResource,   "Help string",
//		I2CResource,    "Help string",
	);

	static void putc(uint8_t c) {
		// send one character out
	}

	static bool ngetc(uint8_t &c) {
		// Check for incoming character. If available, assign it to c and return true
		// Else return false
	}
};
