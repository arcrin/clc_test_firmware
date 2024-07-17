#pragma once
#include "UARTConfig.h"
#include <string_view>


class UARTDriver {
	public:
		constexpr UARTDriver() = default;
		~UARTDriver() = default;	

		virtual void configure(UARTConfig const & current_config) const = 0;

		virtual void unconfigure() const = 0;	

		virtual void IRQHandler() const = 0;

		virtual bool ngetc(uint8_t &c) const = 0;

		virtual bool ngetc(uint8_t) const = 0;

		virtual void enable_tx() const = 0;	

		virtual void disable_tx() const = 0;	

		virtual void enable_rx() const = 0;

		virtual void disable_rx() const = 0;

		/// @brief send a break condition for `nbits` bits	
		/// @param nbits 
		virtual void send_break(uint32_t nbits) const = 0;	

		virtual void start_break() const = 0;	

		virtual void end_break() const = 0;		

		/// @brief wait for transmission to finish
		virtual void flush() const = 0;

		/// @brief receive any pending bytes and clear the RX buffer
		virtual void flush_input() const = 0;

		/// @brief transmit unsigned character
		/// @param c 
		virtual void putc(uint8_t c) const = 0;

		size_t ngets(uint8_t * buf, size_t length) const;

		void putc(char c) const;

		/// @brief transmit string_view
		/// @param s 
		void puts(std::string_view s) const;

		/// @brief transmit character array
		/// @param s 
		/// @param length 
		void puts(const char * s, uint32_t length) const;

		/// @brief transmig unsigned character array
		/// @param s 
		/// @param length 
		void puts(const uint8_t * s, uint32_t length) const;

	protected:
		/// @brief transmit unsigned character, without asserting RTS
		/// @param c 
		virtual void putc_raw(uint8_t c) const = 0;

		/// @brief assert the RTS signal, if it is present
		virtual void assert_rtx() const = 0;

		virtual void deassert_rtx() const = 0;
};