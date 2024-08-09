#pragma once
#include "ADCResource.h"
#include "IOResource.h"

class BoardADC {
    public:
        static void Init() {
            // for (const auto & adc : ADCResourceList) {
            //     adc.getconstref().configure();
            // }
        }
        STATIC_VALS (
            ADCResource,
            // RLYA_FB1, ({ADC1_BASE, 4, {{GPIOA_BASE, 4}, GPIODriver::MODE::ANALOG}}),
            // RLYB_FB1, ({ADC1_BASE, 5, {{GPIOA_BASE, 5}, GPIODriver::MODE::ANALOG}}),
            // RLYA_FB2, ({ADC1_BASE, 6, {{GPIOA_BASE, 6}, GPIODriver::MODE::ANALOG}}),
            // RLYB_FB2, ({ADC1_BASE, 7, {{GPIOA_BASE, 7}, GPIODriver::MODE::ANALOG}}),
            // RLYA_FB3, ({ADC1_BASE, 10, {{GPIOC_BASE, 0}, GPIODriver::MODE::ANALOG}}),
            // RLYB_FB3, ({ADC1_BASE, 11, {{GPIOC_BASE, 1}, GPIODriver::MODE::ANALOG}}),

            // SWA_ON1, ({ADC1_BASE, 4, {{GPIOA_BASE, 4}, GPIODriver::MODE::ANALOG}}),
            // SWB_ON1, ({ADC1_BASE, 5, {{GPIOA_BASE, 5}, GPIODriver::MODE::ANALOG}}),
            // SWA_ON2, ({ADC1_BASE, 6, {{GPIOA_BASE, 6}, GPIODriver::MODE::ANALOG}}),
            // SWB_ON2, ({ADC1_BASE, 7, {{GPIOA_BASE, 7}, GPIODriver::MODE::ANALOG}}),
            // SWA_ON3, ({ADC1_BASE, 10, {{GPIOC_BASE, 0}, GPIODriver::MODE::ANALOG}}),
            // SWB_ON3, ({ADC1_BASE, 11, {{GPIOC_BASE, 1}, GPIODriver::MODE::ANALOG}}),
            // SWA_ON4, ({ADC1_BASE, 12, {{GPIOC_BASE, 2}, GPIODriver::MODE::ANALOG}}),
            // SWB_ON4, ({ADC1_BASE, 13, {{GPIOC_BASE, 3}, GPIODriver::MODE::ANALOG}}),

            ADC04, ({ADC1_BASE, 4, {{GPIOA_BASE, 4}, GPIODriver::MODE::ANALOG}}),
            ADC05, ({ADC1_BASE, 5, {{GPIOA_BASE, 5}, GPIODriver::MODE::ANALOG}}),
            ADC06, ({ADC1_BASE, 6, {{GPIOA_BASE, 6}, GPIODriver::MODE::ANALOG}}),
            ADC07, ({ADC1_BASE, 7, {{GPIOA_BASE, 7}, GPIODriver::MODE::ANALOG}}),
            ADC10, ({ADC1_BASE, 10, {{GPIOC_BASE, 0}, GPIODriver::MODE::ANALOG}}),
            ADC11, ({ADC1_BASE, 11, {{GPIOC_BASE, 1}, GPIODriver::MODE::ANALOG}}),
            ADC12, ({ADC1_BASE, 12, {{GPIOC_BASE, 2}, GPIODriver::MODE::ANALOG}}),
            ADC13, ({ADC1_BASE, 13, {{GPIOC_BASE, 3}, GPIODriver::MODE::ANALOG}}),
        );
};