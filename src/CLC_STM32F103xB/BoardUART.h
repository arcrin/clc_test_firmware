#pragma once
#include "UARTResource.h"
#include "BoardGPIO.h"  
#include "IOResource.h"

class BoardUART {
    public:
        static inline uint8_t DEBUG_RX_BUF[64];

        static void Init();

    static constexpr UARTResource DEBUG_UART{{USART3_BASE, 0x0}, DEBUG_RX_BUF, sizeof(DEBUG_RX_BUF)};

    STATIC_REFS(
        UARTResource, 
        DEBUG, (DEBUG_UART),
    );


    // enum class UARTResource_t { DEBUG, }; 
    // static constexpr SearchableRef<UARTResource> UARTResourceList[] { 
    //     {"DEBUG", {(DEBUG_UART)}}, 
    // }; 
    // static constexpr const UARTResource & GetUARTResource(UARTResource_t resource) {
    //      return UARTResourceList[(uint8_t)resource].getconst(); 
    // } 
    // static constexpr const UARTResource * GetUARTResource(std::string_view Name) { 
    //     if (auto Index = Searchable::SearchPartialUnique(UARTResourceList, sizeof(UARTResourceList) / sizeof(UARTResourceList[0]), Name); Index >= 0) { 
    //         return &UARTResourceList[Index].getconst(); 
    //     } 
    //     return nullptr; 
    // } 
    // static constexpr UARTResource_t GetUARTResourceIndex(std::string_view Name) { 
    //     return (UARTResource_t)Searchable::SearchPartialUnique(UARTResourceList, sizeof(UARTResourceList) / sizeof(UARTResourceList[0]), Name); 
    // }


    static constexpr UARTConfig DEBUG_CONFIG {
        115200, 
        UARTConfig::PARITY::NONE, 
        UARTConfig::STOP::ONE, 
        {{GPIOB_BASE, 10}, GPIODriver::MODE::ALTERNATE}, 
        {{GPIOB_BASE, 11}, GPIODriver::MODE::INPUT}};

    STATIC_REFS(
        UARTConfig,
        DEBUG, (DEBUG_CONFIG)
    );

};