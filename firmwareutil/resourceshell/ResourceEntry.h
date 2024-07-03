#pragma once
#include "macro_enum_map.h"
#include "Searchable.h"
#include "IOResource.h"

#ifdef RESOURCE_SHELL_ENABLE_HELP
#define RESOURCE_MAP_GET_VALUE(cls, help) {#cls, cls::Init, cls::Handler, cls::Task, cls::CmdMap, (sizeof(cls::CmdMap) / sizeof(cls::CmdMap[0])), help},
#else
#define RESOURCE_MAP_GET_VALUE(cls, help) {#cls, cls::Init, cls::Handler, cls::Task, cls::CmdMap, (sizeof(cls::CmdMap) / sizeof(cls::CmdMap[0]))}
#endif

#define RESOURCE_MAP(...) \
MAKE_MAP(static constexpr const ResourceEntry ResourceInfo[], FOR_EACH(RESOURCE_MAP_GET_VALUE, FOR_EACH_HELPER_1, __VA_ARGS__))

struct ResourceEntry : public Searchable {
    public:
        constexpr ResourceEntry(const std::string_view Name, void (*Init)(), CommandHandler_t Handler, void (*Task)(), const CmdEntry * CmdMap, uint32_t CmdMapLength
        #ifdef RESOURCE_SHELL_ENABLE_HELP
            , std::string_view Help
        #endif
        ):
            Searchable(Name),
            Init(Init),
            Handler(Handler), 
            Task(Task),
            CmdMap(CmdMap),
            CmdMapLength(CmdMapLength)
            #ifdef RESOURCE_SHELL_ENABLE_HELP
            ,Help(Help)
            #endif
            {}

        void (*Init)();
        CommandHandler_t Handler;
        void (*Task)(); 
        const CmdEntry * CmdMap;    
        uint32_t CmdMapLength;
        #ifdef RESOURCE_SHELL_ENABLE_HELP
        std::string_view Help;
        #endif

};