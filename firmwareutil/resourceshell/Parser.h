#pragma once
#include "resource_shell_config.h"
#include <cstdint>
#include <string_view>
#include "boost/static_string.hpp"

namespace Parser
{
	/// Converts a string of hexadecimal characters in to an array of their byte values.
	/// \param Input String of hexadecimal characters. Character case is ignored
	/// \param Numbers A buffer to store the byte values into
	/// \param Count How many bytes to convert. Two input characters are read for each output byte.
	/// \return True on success. False if `Input` is shorter than `Count * 2` or if `Input` contains an invalid hexadecimal character
	bool NumbersFromHexString(std::string_view Input, uint8_t *Numbers, size_t Count);

	/// Converts an array of bytes into a hexadecimal string.
	/// \param Numbers An array of bytes to convert
	/// \param Count Number of bytes to convert
	/// \param Output A static_string to append the result string to
	/// \param Reverse If true, `Numbers` will be iterated backwards. This is useful for reversing endianness when `Numbers` points to a type larger than one byte
	// TODO this could be templated on the pointer type to deal with endianness correctly for number sequences > 1
	void HexStringFromNumbers(const void * Numbers, uint32_t Count, boost::static_string <RESOURCE_SHELL_OUTPUT_SIZE> &Output, bool Reverse=false);

	/// Converts a number to its base-10, decimal string representation.
	/// \param Number Number to convert
	/// \param Output A static_string to append the result string to
	/// \param DecimalPlace If greater than 0, a decimal point will be inserted this many places from the right. This
	/// can be used to emulate floating point numbers, by multiplying `Number` by the correct power of ten beforehand
	void DecStringFromNumber(int32_t Number, boost::static_string <RESOURCE_SHELL_OUTPUT_SIZE> &Output, uint8_t DecimalPlace=0);

	/// Convert a number to its hexadecimal string representation.
	/// \tparam T Type of `Number`. The width of this type determines how many hexadecimal characters are output
	/// \param Number Number to convert
	/// \param Output A static_string to append the result string to
	template <typename T>
	void HexFromNumber(T Number, boost::static_string <RESOURCE_SHELL_OUTPUT_SIZE> &Output) {
		// reverse since ARM is little endian
		HexStringFromNumbers((uint8_t*)&Number, sizeof(T), Output, true);
	}

	/// This function contains the internal logic for NumFromString. It should not be used directly. It only exists
	/// to avoid generation of nearly identical code for each templated type
	bool InternalNumFromString(std::string_view Input, int32_t &Number);

	/// Convert a hexadecimal (0x prefix) or integer string representation to an integer.
	/// Floating-point and scientific notation string are not supported.
	/// \tparam T Type of `Number`.
	/// \param Input String to convert
	/// \param Number Resulting number
	/// \return True on success. False if an invalid character was encountered.
	template <typename T>
	bool NumFromString(const std::string_view Input, T &Number) {
		int32_t val;
		bool res = InternalNumFromString(Input, val);
		Number = (T)val;
		return res;
	}
}
