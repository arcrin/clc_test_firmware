#pragma once
#include "IOResource.h"

class GPIOResource : public IOResource {
    public:
        static void Init();
        static CommandHandler(Handler);
        // SHELLERR Handler(
        //     uint32_t Command, 
        //     std::string_view * Args, 
        //     uint32_t NumArgs, 
        //     boost::static_string<1024> &Output, 
        //     uint32_t &Error, 
        //     bool (*InterruptOccurred)(void)
        // );

        CMD_MAP(
            CONF, "<gpio name>. (Re)-configure pin",
            W,    "<gpio name> <0/1>. Set logical pin state",
            R,    "<gpio name>. Read logical pin state",
        );
};

// Above macro expands to the following:  

// enum CMD { 
//     CONF, 
//     W, 
//     R, 
// }; 
// static constexpr CmdEntry CmdMap[]{ 
//     {"CONF",    {"<gpio name>. (Re)-configure pin"}}, 
//     {"W",       {"<gpio name> <0/1>. Set logical pin state"}}, 
//     {"R",       {"<gpio name>. Read logical pin state"}}, 
// };

