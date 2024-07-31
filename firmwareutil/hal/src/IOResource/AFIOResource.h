#pragma
#include "IOResource.h"

class AFIOResource : public IOResource {
    public:
        static void Init();
        static CommandHandler(Handler);
        CMD_MAP(
            CONF, "Remap peripheral",
            RESET, "Reset peripheral mapping",
        );
};