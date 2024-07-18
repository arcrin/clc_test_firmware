#include "ResourceHandler.h"
#include "Parser.h"

InputString ResourceHandler::Line;
OutputString ResourceHandler::Output;
std::string_view ResourceHandler::ArgsHolder[RESOURCE_SHELL_MAX_ARGS];

void ResourceHandler::puts(std::string_view s) {
	return puts((uint8_t *) s.data(), s.size());
}

void ResourceHandler::puts_and_update(std::string_view s, uint16_t &Checksum) {
	ComputeChecksum((uint8_t *) s.data(), s.size(), Checksum);
	return puts((uint8_t *) s.data(), s.size());
}

void ResourceHandler::puts(const uint8_t * s, uint32_t Length) {
	while (Length--) {
		putc(*s);
		s++;
	}
}

uint32_t ResourceHandler::GetArgs() {
	char Delim = ' ';
	uint32_t Index = 0;
	uint32_t Previous = 0;
	uint32_t end = Line.size();
	uint32_t ArgIndex = 0;
	uint32_t State = 0;
	while (1) {
		if (Index >= end || (Index == Previous && ArgIndex == RESOURCE_SHELL_MAX_ARGS) || (Index > Previous && ArgIndex >= (RESOURCE_SHELL_MAX_ARGS - 1))) {
			if (Index > Previous) {
				ArgsHolder[ArgIndex++] = Line.subview(Previous, Index - Previous);
			}
			break;
		}

		switch (State) {
			case 0:
				if (Line[Index] != Delim) {
					State = 1;
					Previous = Index;
				}
				break;

			case 1:
				if (Line[Index] == Delim) {
					ArgsHolder[ArgIndex++] = Line.subview(Previous, Index - Previous);
					State = 0;
				}
				break;

			default:
				break;
		}

		Index++;
	}

	return ArgIndex;
}

void ResourceHandler::ShellPrompt() {
	putc('\n');
	putc('>');
}

void ResourceHandler::Init() {
	for (auto & Resource : ResourceInfo) {
		Resource.Init();
	}
}

// Read characters from the Debug UART buffer, until no characters are immediately available or
// until a non-empty command ending in `\n` is found
// return true if a command is found
void ResourceHandler::ShellTask() {
	static constexpr std::string_view ERASE_SEQUENCE = "\b \b";
#if RESOURCE_SHELL_COMMAND_HISTORY_LENGTH > 0
	static boost::static_string<RESOURCE_SHELL_INPUT_SIZE> CommandHistory[RESOURCE_SHELL_COMMAND_HISTORY_LENGTH];
	static size_t HistoryIdx = 0;
	static size_t RecallIdx = 0;
	static bool CommandModified = false;
#endif
	uint8_t c = 0;

	RunTasks();

	while (ngetc(c)) {
		if (c == '\b') {
			if (!Line.empty()) {
				puts(ERASE_SEQUENCE);
				Line.pop_back();
			}
		}
		else if (c == '\n') {
			putc(c);
			if (Line.empty()) {
				ShellPrompt();
				continue;
			}

#if RESOURCE_SHELL_COMMAND_HISTORY_LENGTH > 0
			if (CommandModified) {
				CommandHistory[HistoryIdx] = Line;
				HistoryIdx = (HistoryIdx + 1) % RESOURCE_SHELL_COMMAND_HISTORY_LENGTH;
			}
			RecallIdx = HistoryIdx;
			CommandModified = false;
#endif

			HandleCommand();
			return;
		}
#if RESOURCE_SHELL_COMMAND_HISTORY_LENGTH > 0
		else if (c == '!') {
			// erase current line
			uint8_t len = Line.size();
			while (len) {
				puts(ERASE_SEQUENCE);
				len--;
			}

			RecallIdx = (RecallIdx + RESOURCE_SHELL_COMMAND_HISTORY_LENGTH - 1) % RESOURCE_SHELL_COMMAND_HISTORY_LENGTH;
			// copy command to line
			Line = CommandHistory[RecallIdx];

			// and print it out
			puts(Line);
			CommandModified = false;
		}
#endif
		else if (Line.size() < Line.capacity()) {
#if RESOURCE_SHELL_COMMAND_HISTORY_LENGTH > 0
			CommandModified = true;
#endif
			Line += (char) c;
			putc(c);
		}
	}
}

void ResourceHandler::RunTasks() {
	for (auto & Resource : ResourceInfo) {
		if (Resource.Task) {
			Resource.Task();
		}
	}
}

void ResourceHandler::HandleCommand() {
	uint32_t Error = 0;
	bool UseChecksum = false;
	uint16_t Checksum = CHECKSUM_INIT;

	SHELLERR Result = Handle(Error, UseChecksum);

	if (Result != SHELLERR::OK) {
		puts_and_update("ERROR: ", Checksum);
		puts_and_update(SHELLERRStrings[(int)Result], Checksum);
		puts_and_update(": 0x", Checksum);

		char symbols[20];
		uint8_t index = 0;

		while (Error || index == 0) {
			symbols[index++] = "0123456789ABCDEF"[Error & 0x0F];
			Error >>= 4;
		}

		while (index--) {
			ComputeChecksum((uint8_t *)&symbols[index], 1, Checksum);
			putc(symbols[index]);
		}

		puts_and_update(": ", Checksum);
	}

	puts_and_update(Output, Checksum);

	Line.clear();
	Output.clear();

	if (UseChecksum) {
		uint8_t crcs[2] = {(uint8_t)(Checksum >> 8), (uint8_t)Checksum};
		Parser::HexStringFromNumbers(crcs, 2, Output);

		putc(CHECKSUM_INDICATOR);
		puts(Output);
		Output.clear();
	}

	ShellPrompt();
}

SHELLERR ResourceHandler::Handle(uint32_t &Error, bool &UseChecksum) {
	// <resource> <command> [args ...] ['~' crc16 as hex string]
	if (Line.size() >= 5 && *(Line.end()-5) == CHECKSUM_INDICATOR) {
		uint8_t expected_crc[2];
		if (!Parser::NumbersFromHexString(Line.end()-4, expected_crc, 2)) {
			return SHELLERR::CHECKSUM;
		}

		// Remove the CRC and indicator
		Line.pop_back();
		Line.pop_back();
		Line.pop_back();
		Line.pop_back();
		Line.pop_back();

		uint16_t actual_crc = CHECKSUM_INIT;
		ComputeChecksum((uint8_t *) Line.data(), Line.size(), actual_crc);
		if (actual_crc != ((expected_crc[0] << 8) | (expected_crc[1]))) {
			return SHELLERR::CHECKSUM;
		}
		UseChecksum = true;
	}
	uint32_t NumArgs = GetArgs();

	if (!NumArgs) {
		// empty input. ignore
		return SHELLERR::OK;
	}

	if (ArgsHolder[0] == "help") {
		// special case. Gather all resource and command info
		for (auto & Resource : ResourceInfo) {
			uint32_t CmdMapLength = Resource.CmdMapLength;

			puts(Resource.GetName());
#ifdef RESOURCE_SHELL_ENABLE_HELP
			puts(": ");
			puts(Resource.Help);
#endif
			putc('\n');

			const CmdEntry *CmdMap = Resource.CmdMap;
			for (uint32_t c = 0; c < CmdMapLength; c++) {
				auto & Entry = CmdMap[c];
				putc(' ');
				puts(Entry.GetName());
#ifdef RESOURCE_SHELL_ENABLE_HELP
				puts(": ");
				puts(Entry.Help);
#endif
				putc('\n');
			}
		}
		return SHELLERR::OK;
	}

	if (NumArgs < 2) {
		return SHELLERR::ARGUMENT;
	}

	auto Key = SEARCH_PARTIAL_UNIQUE(ResourceInfo, ArgsHolder[0]);
	if (Key < 0) {
		return SHELLERR::RESOURCE;
	}

	std::string_view Command = ArgsHolder[1];
	const ResourceEntry &Info = ResourceInfo[Key];

	if (!Info.Handler) {
		return SHELLERR::RESOURCE;
	}

	NumArgs -= 2;

	auto CmdKey = Searchable::SearchPartialUnique(Info.CmdMap, Info.CmdMapLength, Command);
	if (CmdKey < 0) {
		return SHELLERR::IMPLEMENTATION;
	}

	return Info.Handler(CmdKey, &ArgsHolder[2], NumArgs, Output, Error, InterruptOccurred);
}

bool ResourceHandler::InterruptOccurred() {
	// If any character is received during command processing, we consider it a command interrupt
	uint8_t c = 0;
	return ngetc(c) && c == 5;
}

void ResourceHandler::ComputeChecksum(const uint8_t * Data, size_t Length, uint16_t &Checksum) {
	while (Length--) {
		uint8_t newByte = *Data;
		for (uint8_t i = 0; i < 8; i++) {
			if (((Checksum & 0x8000) >> 8) ^ (newByte & 0x80)) {
				Checksum = (Checksum << 1) ^ CHECKSUM_POLY;
			} else {
				Checksum = (Checksum << 1);
			}

			newByte <<= 1;
		}
		Data++;
	}
}
