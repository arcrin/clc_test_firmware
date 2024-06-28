#pragma once
#include <cstdint>
#include "USBHAL.h"

/// An instance of this class represents a USB peripheral
class USBDriver {
public:
	constexpr USBDriver(USBHAL hal) :
	hal(hal)
	{}
	USBDriver() = delete;
	~USBDriver() = default;

	/// Common initialization of the USB system
	static void DriverInit();

	/// (Re-)configure the USB peripheral
	void configure() const;

	/// USB interrupt handler
	void IRQHandler() const;

protected:
	/// Enable the USB peripheral, causing it to enumerate on the host
	void enable() const;

	/// Disable the USB peripheral, causing a disconnect on the host
	void disable() const;

public:
	static constexpr uint8_t PID_OUT   = 0x1;
	static constexpr uint8_t PID_IN    = 0x9;
	static constexpr uint8_t PID_SOF   = 0x5;
	static constexpr uint8_t PID_SETUP = 0xd;

	static constexpr uint8_t ENDP0_SIZE = 64;
	static constexpr uint8_t ENDP1_SIZE = 64;

	struct dev_descriptor_t {
		uint8_t bLength;
		uint8_t bDescriptorType;
		uint16_t bcdUSB;
		uint8_t bDeviceClass;
		uint8_t bDeviceSubClass;
		uint8_t bDeviceProtocol;
		uint8_t bMaxPacketSize0;
		uint16_t idVendor;
		uint16_t idProduct;
		uint16_t bcdDevice;
		uint8_t iManufacturer;
		uint8_t iProduct;
		uint8_t iSerialNumber;
		uint8_t bNumConfigurations;
	};

	struct int_descriptor_t {
		uint8_t bLength;
		uint8_t bDescriptorType;
		uint8_t bInterfaceNumber;
		uint8_t bAlternateSetting;
		uint8_t bNumEndpoints;
		uint8_t bInterfaceClass;
		uint8_t bInterfaceSubClass;
		uint8_t bInterfaceProtocol;
		uint8_t iInterface;
	};

	struct cfg_descriptor_t {
		uint8_t bLength;
		uint8_t bDescriptorType;
		uint16_t wTotalLength;
		uint8_t bNumInterfaces;
		uint8_t bConfigurationValue;
		uint8_t iConfiguration;
		uint8_t bmAttributes;
		uint8_t bMaxPower;
		int_descriptor_t interfaces[1];
	};

	struct descriptor_entry_t {
		uint16_t wValue;
		uint16_t wIndex;
		const void* addr;
		uint8_t length;
	};

	struct setup_t {
		union {
			struct {
				uint8_t bmRequestType;
				uint8_t bRequest;
			};
			uint16_t wRequestAndType;
		};
		uint16_t wValue;
		uint16_t wIndex;
		uint16_t wLength;
	};

private:
	const USBHAL hal;
};


