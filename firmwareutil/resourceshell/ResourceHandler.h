#pragma once
#include "config.h"
#include "ResourceMap.h"

class ResourceHandler : public ResourceMap {
    public:
        ResourceHandler() = default;
        ~ResourceHandler() = default;

        static void Init();
        static void ShellTask();

    private:
        static constexpr char CHECKSUM_INDICATOR = '~';
        static constexpr uint16_t CHECKSUM_POLY = 0x1021;
        static constexpr uint16_t CHECKSUM_INIT = 0xFFFF;

        static InputString Line;
        static OutputString Output;
        static std::string_view ArgsHolder[RESOURCE_SHELL_MAX_ARGS];

        static bool InterruptOccurred;
        static void RunTasks();
        static uint32_t GetArgs();
        static void HandleCommand();    
        static SHELLERR Handle(uint32_t &Error, bool &UseChecksum);
        static void ShellPrompt();
        static void puts(std::string_view s);
        static void puts(const uint8_t* s, uint32_t Length);
        static void puts_and_update(std::string_view s, uint16_t &Checksum);
        static void ComputeChecksum(const uint8_t* Data, size_t Length, uint16_t &Checksum);
};