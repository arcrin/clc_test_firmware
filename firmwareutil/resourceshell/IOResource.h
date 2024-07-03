#pragma once
#include "resource_shell_config.h"
#include "ShellErr.h"
#include <string_view>
#include "boost/static_string.hpp"
#include "macro_enum_map.h"
#include "Searchable.h"

extern uint8_t Temp[RESOURCE_SHELL_TEMP_ARRAY_SIZE];

// the parameter "name" is the name of the handler function, e.g. "GPIOHandler"
#define CommandHandler(name)                                    \
    SHELLERR name(uint32_t Command,                             \
    std::string_view Args[],                                    \
    uint32_t NumArgs,                                            \
    boost::static_string<RESOURCE_SHELL_OUTPUT_SIZE> &Output,   \
    uint32_t &Error, bool (*InterruptOccurred)(void))

typedef CommandHandler((*CommandHandler_t));

class CmdEntry : public Searchable {
    friend class ResourceHandler;
    public:
        constexpr CmdEntry(std::string_view Name, 
                        #ifdef RESOURCE_SHELL_ENABLE_HELP
                        std::string_view Help
                        #endif
                        ) : Searchable(Name)
                        #ifdef RESOURCE_SHELL_ENABLE_HELP
                        ,Help(Help) 
                        #endif
                        {}

    private:
        #ifdef RESOURCE_SHELL_ENABLE_HELP
        std::string_view Help;
        #endif
};

#ifdef RESOURCE_SHELL_ENABLE_HELP
#define CMD_MAP(...) ENUM_MAP(enum CMD, static constexpr CmdEntry CmdMap[], 1, NAMED_VALUE_HELPER, __VA_ARGS__)
#else
#define CMD_MAP(...) ENUM_MAP(enum CMD, static constexpr CmdEntry CmdMap[], 1, NAME_ONLY_HELPER, __VA_ARGS__)
#endif

#define STATIC_REFS(class_name, ...) \
ENUM_MAP(enum class class_name##_t, static constexpr SearchableRef<class_name> class_name##List[], 1, NAMED_VALUE_HELPER, __VA_ARGS__); \
static constexpr const class_name & Get##class_name(class_name##_t resource) {\
    return class_name##List[(uint8_t)resource].getconst();\
}\
static constexpr const class_name * Get##class_name(std::string_view Name) {\
    if (auto Index = SEARCHA_PARTIAL_UNIQUE(class_name##List, Name); Index >= 0) {\
        return &class_name##List[Index].getconst();\
    }\
    return nullptr;\
}\
static constexpr class_name##_t Get##class_name##Index(std::string_view Name) {\
    return (class_name##_t)SEARCHABLE_PARTIAL_UNIQUE(class_name##List, Name);\
}

#define STATIC_VALS(class_name, ...) \
ENUM_MAP(enum class class_name##_t, static constexpr SearchableVal<class_name> class_name##List[], 1, NAMED_BRACED_VALUE_HELPER, __VA_ARGS__); \
static constexpr const class_name & Get##class_name(class_name##_t resource) {                                                                 \
	return class_name##List[(uint8_t)resource].getconstref();                                                                                  \
}                                                                                                                                              \
static constexpr const class_name * Get##class_name(std::string_view Name) {                                                                   \
	if (auto Index = SEARCH_PARTIAL_UNIQUE(class_name##List, Name); Index >= 0) {                                                              \
		return &class_name##List[Index].getconstref();                                                                                         \
	}                                                                                                                                          \
	return nullptr;                                                                                                                            \
}                                                                                                                                              \
static constexpr class_name##_t Get##class_name##Index(std::string_view Name) {                                                                \
	return (class_name##_t)SEARCH_PARTIAL_UNIQUE(class_name##List, Name);                                                                      \
}

#define NAMED_ENUM(type_name, ...) \
ENUM_MAP0(enum class type_name, static constexpr Searchable type_name##List[], NAME_ONLY_HELPER, __VA_ARGS__); \
static constexpr type_name##_t Get##type_name##Index(std::string_view Name) {\
    return (type_name##_t)SEARCH_PARTIAL_UNIQUE(type_name##List, Name);\
}

class IOResource {
    public:
        constexpr IOResource() = default;
        ~IOResource() = default;

        static void Init();
        static CommandHandler(Handler) {return SHELLERR::IMPLEMENTATION;}
        static inline constexpr void (*Task)() = nullptr;
    protected:
        #ifdef RESOURCE_SHELL_TEMP_ARRAY_SIZE
            static inline uint8_t (&TempArray)[RESOURCE_SHELL_TEMP_ARRAY_SIZE] = Temp;
        #endif
};