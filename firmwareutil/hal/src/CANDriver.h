#pragma once
#include "GPIODriver.h"
#include "CANHAL.h"
#include <utility>

/// An instance of this class represents the CAN peripheral and associated IO pins
class CANDriver {
public:
	/// Create a CAN instance
	/// \param hal Processor specific information about the CAN peripheral
	/// \param TX IO pin to use for TX. Must be constructed with the appropriate alternate function
	/// \param RX IO pin to use for RX. Must be constructed with the appropriate alternate function
	constexpr CANDriver(CANHAL hal, const GPIODriver && TX, const GPIODriver && RX) :
		hal(hal), TX(std::move(TX)), RX(std::move(RX))
	{}
	CANDriver() = delete;
	~CANDriver() = default;

	/// Common initialization of the CAN system
	static void DriverInit();

	/// (Re-)configure the CAN peripheral
	// TODO take bitrate, etc as parameters
	void configure() const;

	/// Send a CAN message of up to 8 bytes, with the specified ID
	/// \param Data Data to send
	/// \param Length Length of Data
	/// \param ID ID to send
	void Write(uint8_t Data[8], uint8_t Length, uint32_t ID) const;

	/// Receive a CAN message
	/// \param[out] Data Buffer to receive into
	/// \param[out] Length Length of received data
	/// \param[out] ID ID received
	/// \param[out] Timestamp Timestamp when message was received
	/// \param[out] Remote Whether this is a remote message
	/// \return True if a message was available, false otherwise
	bool Read(uint8_t Data[8], uint8_t &Length, uint32_t &ID, uint16_t &Timestamp, bool &Remote) const;

	/// Enable the transceiver
	void Enable() const;

	/// Disable the transceiver
	void Disable() const;

private:
	const CANHAL hal;
	const GPIODriver TX;
	const GPIODriver RX;
};
