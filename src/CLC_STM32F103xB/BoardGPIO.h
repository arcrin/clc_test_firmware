#pragma once
#include "GPIODriver.h"
#include "IOResource.h"

class BoardGPIO {

    public:
        static void Init();

        STATIC_VALS(
            GPIODriver, 
            LED_RED, ({GPIOE_BASE, 0}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            LED_GREEN, ({GPIOE_BASE, 1}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),   
            SW311_LED, ({GPIOE_BASE, 8}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            SW301_LED, ({GPIOE_BASE, 9}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            SW312_LED, ({GPIOE_BASE, 10}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            SW302_LED, ({GPIOE_BASE, 11}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            SW313_LED, ({GPIOE_BASE, 12}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            SW303_LED, ({GPIOE_BASE, 13}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            // SW7_LED, ({GPIOE_BASE, 14}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            // SW8_LED, ({GPIOE_BASE, 15}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE)
            SW311_PB, ({GPIOC_BASE, 8}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::UP),
            SW301_PB, ({GPIOC_BASE, 9}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::UP),
            SW312_PB, ({GPIOC_BASE, 10}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::UP),
            SW302_PB, ({GPIOC_BASE, 11}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::UP),
            SW313_PB, ({GPIOC_BASE, 12}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::UP),
            SW303_PB, ({GPIOC_BASE, 13}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::UP),
            
        );

    //     enum class GPIODriver_t{
    //         LED_RED, 
    //         LED_GREEN, 
    //         SW1_LED, 
    //         SW2_LED, 
    //         SW3_LED, 
    //         SW4_LED, 
    //         SW5_LED, 
    //         SW6_LED, 
    //         SW7_LED, 
    //         SW8_LED, 
    //     }; 
    
    // static constexpr SearchableVal<GPIODriver> GPIODriverList[]{ 
    //     {"LED_RED", 
    //         {
    //             {((0x40000000UL + 0x00010000UL) + 0x00001800UL), 0}, 
    //             GPIODriver::MODE::OUTPUT, 
    //             0, 
    //             false, 
    //             false, 
    //             GPIODriver::TYPE::PUSH_PULL, 
    //             GPIODriver::SPEED::MEDIUM, 
    //             GPIODriver::PULL::NONE}
    //         },
    //     {"LED_GREEN", {{((0x40000000UL + 0x00010000UL) + 0x00001800UL), 1}, GPIODriver::MODE::OUTPUT, 0, false, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE}},
    //     {"SW1_LED", {{((0x40000000UL + 0x00010000UL) + 0x00001800UL), 8}, GPIODriver::MODE::OUTPUT, 0, false, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE}}, 
    //     {"SW2_LED", {{((0x40000000UL + 0x00010000UL) + 0x00001800UL), 9}, GPIODriver::MODE::OUTPUT, 0, false, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE}}, 
    //     {"SW3_LED", {{((0x40000000UL + 0x00010000UL) + 0x00001800UL), 10}, GPIODriver::MODE::OUTPUT, 0, false, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE}}, 
    //     {"SW4_LED", {{((0x40000000UL + 0x00010000UL) + 0x00001800UL), 11}, GPIODriver::MODE::OUTPUT, 0, false, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE}}, 
    //     {"SW5_LED", {{((0x40000000UL + 0x00010000UL) + 0x00001800UL), 12}, GPIODriver::MODE::OUTPUT, 0, false, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE}}, 
    //     {"SW6_LED", {{((0x40000000UL + 0x00010000UL) + 0x00001800UL), 13}, GPIODriver::MODE::OUTPUT, 0, false, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE}}, 
    //     {"SW7_LED", {{((0x40000000UL + 0x00010000UL) + 0x00001800UL), 14}, GPIODriver::MODE::OUTPUT, 0, false, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE}}, 
    //     {"SW8_LED", {{((0x40000000UL + 0x00010000UL) + 0x00001800UL), 15}, GPIODriver::MODE::OUTPUT, 0, false, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE}}, }; 
        
        
    // static constexpr const GPIODriver & GetGPIODriver(GPIODriver_t resource) { 
    //     return GPIODriverList[(uint8_t)resource].getconstref(); 
    // }

    // static constexpr const GPIODriver * GetGPIODriver(std::string_view Name) { 
    //     if (auto Index = Searchable::SearchPartialUnique(GPIODriverList, sizeof(GPIODriverList) / sizeof(GPIODriverList[0]), Name); Index >= 0) { 
    //         return &GPIODriverList[Index].getconstref(); } return nullptr; 
    //     }
    // static constexpr GPIODriver_t GetGPIODriverIndex(std::string_view Name) {
    //      return (GPIODriver_t)Searchable::SearchPartialUnique(GPIODriverList, sizeof(GPIODriverList) / sizeof(GPIODriverList[0]), Name); 
    // }
};

