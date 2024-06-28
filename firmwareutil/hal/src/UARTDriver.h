#pragma once
#include "util.h"
#include "UARTConfig.h"
#include "UARTHAL.h"
#include "RingBuffer.h"
#include <string_view>
#include <utility>

/// An instance of this class represents the UART peripheral only
/// The peripheral is routed to IO pins by using a UARTConfig instance
class UARTDriver {
	// TODO: UPDIDriver accesses `current_config` to change TX mode
	friend class UPDIDriver;
public:
	/// Create a UART instance
	/// \param hal Processor specific information about the UART peripheral
	/// \param Buffer Data buffer to use for reception
	/// \param BufferSize Size of the reception buffer
	constexpr UARTDriver(UARTHAL hal, uint8_t *Buffer, uint32_t BufferSize) :
		hal(std::move(hal)), Buffer(Buffer, BufferSize)
		{}
	UARTDriver() = delete;
	~UARTDriver() = default;

	/// Common initialization of the UART system
	static void DriverInit();

	/// (Re-)configure the UART peripheral using the settings provided in config, and route it to the IO pins
	///  provided in config
	/// \param config Configuration to use
	void configure(UARTConfig const & current_config) const;

	/// Receive interrupt handler. This should put bytes into the RX buffer
	void IRQHandler() const;

	/// Non-blocking receive character from RX buffer
	/// \param[out] c Received character
	/// \return True if a character was available. Otherwise, false is returned and `c` is invalid
	bool ngetc(uint8_t &c) const;

	/// Non-blocking receive string from RX buffer
	/// \param buf Buffer to copy the string into
	/// \param Length Maximum number of characters to receive
	/// \return The number of characters that were available
	size_t ngets(uint8_t * buf, size_t Length) const;

	/// Transmit character
	/// \param c Character to send out
	void putc(char c) const;

	/// Transmit unsigned character
	/// \param c Character to send out
	void putc(uint8_t c) const;

	/// Enable / configure the TX IO pin for UART transmission
	void enable_tx() const;

	/// Disable / tri-state the TX IO pin
	void disable_tx() const;

	/// Enable / configure the RX IO pin for UART reception
	void enable_rx() const;

	/// Disable / tri-state the RX IO pin
	void disable_rx() const;

	/// Wait for transmission to finish
	void flush() const;

	/// Receive any pending bytes and clear the RX buffer
	void flush_input() const;

	/// Transmit string_view
	/// \param s String to send
	void puts(std::string_view s) const;

	/// Transmit character array
	/// \param s Pointer to first character
	/// \param Length Number of characters to transmit
	void puts(const char * s, uint32_t Length) const;

	/// Transmit unsigned character array
	/// \param s Pointer to first character
	/// \param Length Number of characters to transmit
	void puts(const uint8_t * s, uint32_t Length) const;

private:
	/// Transmit unsigned character, without asserting RTS
	/// \param c Character to send out
	void putc_raw(uint8_t c) const;

	/// Assert the RTS signal, if it is present
	void assert_rts() const;

	/// De-assert the RTS signal, if it is present
	void deassert_rts() const;

	const UARTHAL hal;
	mutable const UARTConfig * current_config {};
	const RingBuffer<uint8_t> Buffer;
};
