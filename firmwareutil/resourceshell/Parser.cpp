#include "Parser.h"

namespace Parser {
	static bool ParseHexNibble(char c, uint8_t &Number);

	bool NumbersFromHexString(const std::string_view Input, uint8_t *Numbers, size_t Count) {
		//string should have an even number of characters since each byte is 2 characters
		if ((Count * 2) > Input.size()) {
			return false;
		}

		size_t x = 0;
		for (x = 0; x < Count * 2; x += 2)
		{
			uint8_t vh = 0;
			uint8_t vl = 0;
			if (!ParseHexNibble(Input[x], vh)) return false;
			if (!ParseHexNibble(Input[x+1], vl)) return false;
			*Numbers = ((vh << 4) | vl);
			Numbers++;
		}
		return true;
	}

	void HexStringFromNumbers(const void * Numbers, uint32_t Length, boost::static_string <RESOURCE_SHELL_OUTPUT_SIZE> &Output, bool Reverse) {
		constexpr char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

		auto *Ptr = static_cast<const uint8_t *>(Numbers);

		if (Reverse) {
			for (int32_t i = Length-1; i >= 0; i--) {
				Output += hex[Ptr[i] >> 4];
				Output += hex[Ptr[i] & 0x0F];
			}
		} else {
			for (uint32_t i = 0; i < Length; i++) {
				Output += hex[Ptr[i] >> 4];
				Output += hex[Ptr[i] & 0x0F];
			}
		}
	}

	//// Use `DecimalPlace` to represent a floating point number.
	//// To show '23.456', pass `Number=23456`, `DecimalPlace=3`
	void DecStringFromNumber(int32_t Number, boost::static_string <RESOURCE_SHELL_OUTPUT_SIZE> &Output, uint8_t DecimalPlace) {
		// TODO the size of `symbols` needs to account for maximum value of DecimalPlace
		char symbols[20];
		uint8_t index = 0;

		if (Number < 0) {
			Output += '-';
			Number *= -1;
		}

		if (DecimalPlace) {
			DecimalPlace++;
		}

		while (Number || index == 0 || DecimalPlace) {
			if (DecimalPlace) {
				DecimalPlace--;
				if (!DecimalPlace) {
					symbols[index++] = '.';
				}
			}

			symbols[index++] = (char)('0' + (Number % 10));
			Number /= 10;
		}

		while (index--) {
			Output += symbols[index];
		}
	}

	static bool ParseHexNibble(const char c, uint8_t &Number) {
		switch (c) {
			case '0': Number = 0; return true;
			case '1': Number = 1; return true;
			case '2': Number = 2; return true;
			case '3': Number = 3; return true;
			case '4': Number = 4; return true;
			case '5': Number = 5; return true;
			case '6': Number = 6; return true;
			case '7': Number = 7; return true;
			case '8': Number = 8; return true;
			case '9': Number = 9; return true;
			case 'a':
			case 'A': Number = 10; return true;
			case 'b':
			case 'B': Number = 11; return true;
			case 'c':
			case 'C': Number = 12; return true;
			case 'd':
			case 'D': Number = 13; return true;
			case 'e':
			case 'E': Number = 14; return true;
			case 'f':
			case 'F': Number = 15; return true;
			default: return false;
		}
	}

	bool InternalNumFromString(std::string_view Input, int32_t &Number) {
		bool is_hex = false;
		bool is_neg = false;
		size_t len = Input.size();
		Number = 0;
		const char * ptr = Input.data();

		if (!len) {
			return false;
		}

		switch (*ptr) {
			case 0:
				return false;
			case '-':
				is_neg = true;
				ptr++;
				len--;
				break;
			case '0':
				if ((len > 1) && *(ptr+1) == 'x') {
					is_hex = true;
					ptr += 2;
					len -= 2;
				}
				break;
			default:
				break;
		}

		uint8_t v;
		while (len && (v = *ptr)) {
			if (is_hex) {
				if (!ParseHexNibble(v, v)) return false;
				Number = (Number << 4) | v;
			} else {
				if (v < '0') return false;
				v -= '0';
				if (v > 9) return false;

				Number = (Number * 10) + v;
			}
			ptr++;
			len--;
		}
		if (is_neg) {
			Number = -Number;
		}
		return true;
	}
}
