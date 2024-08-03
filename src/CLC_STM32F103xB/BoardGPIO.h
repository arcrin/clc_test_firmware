#pragma once
#include "GPIODriver.h"
#include "IOResource.h"

class BoardGPIO {

    public:
        static void Init();

        STATIC_VALS(
            GPIODriver, 
            LED_RED, ({GPIOE_BASE, 0}, GPIODriver::MODE::OUTPUT, 0, true, true, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            LED_GREEN, ({GPIOE_BASE, 1}, GPIODriver::MODE::OUTPUT, 0, true, true, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),   
            LED_SW311, ({GPIOE_BASE, 8}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            LED_SW301, ({GPIOE_BASE, 9}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            LED_SW312, ({GPIOE_BASE, 10}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            LED_SW302, ({GPIOE_BASE, 11}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            LED_SW313, ({GPIOE_BASE, 12}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            LED_SW303, ({GPIOE_BASE, 13}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),

            LED_SYS_GREEN, ({GPIOE_BASE, 4}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            LED_SYS_RED, ({GPIOE_BASE, 5}, GPIODriver::MODE::OUTPUT, 0, false, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),

            // SW7_LED, ({GPIOE_BASE, 14}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            // SW8_LED, ({GPIOE_BASE, 15}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),

            ADDR_11, ({GPIOE_BASE, 8}, GPIODriver::MODE::INPUT, 0, false, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            ADDR_12, ({GPIOE_BASE, 9}, GPIODriver::MODE::INPUT, 0, false, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            ADDR_14, ({GPIOE_BASE, 10}, GPIODriver::MODE::INPUT, 0, false, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            ADDR_18, ({GPIOE_BASE, 11}, GPIODriver::MODE::INPUT, 0, false, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            ADDR_01, ({GPIOE_BASE, 12}, GPIODriver::MODE::INPUT, 0, false, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            ADDR_02, ({GPIOE_BASE, 13}, GPIODriver::MODE::INPUT, 0, false, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            ADDR_04, ({GPIOE_BASE, 14}, GPIODriver::MODE::INPUT, 0, false, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            ADDR_08, ({GPIOE_BASE, 15}, GPIODriver::MODE::INPUT, 0, false, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),


            CAN_ERR, ({GPIOE_BASE, 2}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            CAN_RX, ({GPIOA_BASE, 11}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::OPEN_DRAIN, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            CAN_RX_DEBUG, ({GPIOA_BASE, 11}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),

            SW311_PB, ({GPIOC_BASE, 8}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::UP),
            SW301_PB, ({GPIOC_BASE, 9}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::UP),
            SW312_PB, ({GPIOC_BASE, 10}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::UP),
            SW302_PB, ({GPIOC_BASE, 11}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::UP),
            SW313_PB, ({GPIOC_BASE, 12}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::UP),
            SW303_PB, ({GPIOC_BASE, 13}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::UP),
            
            RLYA_ON1, ({GPIOD_BASE, 0}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            RLYA_OFF1, ({GPIOD_BASE, 1}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            RLYB_ON1, ({GPIOD_BASE, 2}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            RLYB_OFF1, ({GPIOD_BASE, 3}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            RLYA_ON2, ({GPIOD_BASE, 4}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            RLYA_OFF2, ({GPIOD_BASE, 5}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            RLYB_ON2, ({GPIOD_BASE, 6}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            RLYB_OFF2, ({GPIOD_BASE, 7}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            RLYA_ON3, ({GPIOD_BASE, 8}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            RLYA_OFF3, ({GPIOD_BASE, 9}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            RLYB_ON3, ({GPIOD_BASE, 10}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            RLYB_OFF3, ({GPIOD_BASE, 11}, GPIODriver::MODE::OUTPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),

            SWA_ON1, ({GPIOA_BASE, 3}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            SWA_OFF1, ({GPIOA_BASE, 2}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            SWB_ON1, ({GPIOA_BASE, 1}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            SWB_OFF1, ({GPIOA_BASE, 0}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            SWA_ON2, ({GPIOB_BASE, 9}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            SWA_OFF2, ({GPIOB_BASE, 8}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            SWB_ON2, ({GPIOC_BASE, 7}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            SWB_OFF2, ({GPIOC_BASE, 6}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            SWA_ON3, ({GPIOD_BASE, 15}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            SWA_OFF3, ({GPIOD_BASE, 14}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            SWB_ON3, ({GPIOD_BASE, 13}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
            SWB_OFF3, ({GPIOD_BASE, 12}, GPIODriver::MODE::INPUT, 0, true, false, GPIODriver::TYPE::PUSH_PULL, GPIODriver::SPEED::MEDIUM, GPIODriver::PULL::NONE),
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

