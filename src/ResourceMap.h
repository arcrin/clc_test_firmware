#pragma once
#include "ResourceEntry.h"
#include "GPIOResource.h"
#include "UARTResource.h"
#include "AFIOResource.h"
#include "ADCResource.h"
#include "SPIResource.h"
#include "CANResource.h"
#include "BoardUART.h"

class ResourceMap {
    public: 
        ResourceMap() = default;
        ~ResourceMap() = default;   
    
    protected:
        RESOURCE_MAP(
            GPIOResource, "",
            UARTResource, "",
            AFIOResource, "",   
            ADCResource, "",    
            SPIResource, "",
            CANResource, ""
        );
        // Above macro expands to the following:
        // static constexpr const ResourceEntry ResourceInfo[] { 
        //     {"GPIOResource", GPIOResource::Init, GPIOResource::Handler, GPIOResource::Task, GPIOResource::CmdMap, (sizeof(GPIOResource::CmdMap) / sizeof(GPIOResource::CmdMap[0])), ""}, 
        //     {"UARTResource", UARTResource::Init, UARTResource::Handler, UARTResource::Task, UARTResource::CmdMap, (sizeof(UARTResource::CmdMap) / sizeof(UARTResource::CmdMap[0])), ""}, 
        // };

    static void putc(uint8_t c) {
       static constexpr const UARTResource & DebugUART = BoardUART::GetUARTResource(BoardUART::UARTResource_t::DEBUG);
       DebugUART.putc(c);   
    }

    static bool ngetc(uint8_t &c) {
        static constexpr const UARTResource & DebugUART = BoardUART::GetUARTResource(BoardUART::UARTResource_t::DEBUG);
        return DebugUART.ngetc(c);
    }
    
};
