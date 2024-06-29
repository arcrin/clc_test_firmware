#pragma once
#include "IOResource.h"

class GPIOResource : public IOResource {
    public:
        static void Init();
        static CommandHandler(Handler);

        CMD_MAP(
            CONF, "<gpio name>. (Re)-configure pin",
            W,    "<gpio name> <0/1>. Set logical pin state",
            R,    "<gpio name>. Read logical pin state",
        );
};

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