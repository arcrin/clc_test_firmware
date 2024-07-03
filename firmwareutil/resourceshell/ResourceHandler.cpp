#include "ResourceHandler.h"
#include "Parser.h"

InputString ResourceHandler::Line;
OutputString ResourceHandler::Output;  
std::string_view ResourceHandler::ArgsHolder[RESOURCE_SHELL_MAX_ARGS];

void ResourceHandler::puts(std::string_view s) {
    return puts((uint8_t*) s.data(), s.size());
}

void ResourceHandler::puts_and_update(std::string_view s, uint16_t &Checksum) {
    ComputeChecksum((uint8_t *) s.data(), s.size(), Checksum);
    return puts((uint8_t *) s.data(), s.size());
}

void ResourceHandler::puts(const uint8_t* s, uint32_t Length) {
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

    // TODO
}