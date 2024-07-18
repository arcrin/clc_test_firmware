#pragma once
#include "ResourceEntry.h"
#include "GPIOResource.h"
#include "UARTResource.h"
#include "BoardUART.h"

class ResourceMap {
    public: 
        ResourceMap() = default;
        ~ResourceMap() = default;   
    
    protected:
        RESOURCE_MAP(
            GPIOResource, "",
            UARTResource, "",
        );

    static void putc(uint8_t c) {
       static constexpr const UARTResource & DebugUART = BoardUART::GetUARTResource(BoardUART::UARTResource_t::DEBUG);
       DebugUART.putc(c);   
    }

    static void ngetc(uint8_t &c) {
        static constexpr const UARTResource & DebugUART = BoardUART::GetUARTResource(BoardUART::UARTResource_t::DEBUG);
        DebugUART.ngetc(c);
    }
    
};
