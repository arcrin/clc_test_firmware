#pragma once
#include "ADCResource.h"
#include "IOResource.h"

class BoardADC {
    public:
        static void Init() {
            for (const auto & adc : ADCResourceList) {
                adc.getconstref().configure();
            }
        }
        STATIC_VALS (
            ADCResource,
            RLYA_FB1, ({ADC1_BASE, 4, {{GPIOA_BASE, 4}, GPIODriver::MODE::ANALOG}}),
            RLYB_FB1, ({ADC1_BASE, 5, {{GPIOA_BASE, 5}, GPIODriver::MODE::ANALOG}}),
            RLYA_FB2, ({ADC1_BASE, 6, {{GPIOA_BASE, 6}, GPIODriver::MODE::ANALOG}}),
            RLYB_FB2, ({ADC1_BASE, 7, {{GPIOA_BASE, 7}, GPIODriver::MODE::ANALOG}}),
            RLYA_FB3, ({ADC1_BASE, 10, {{GPIOC_BASE, 0}, GPIODriver::MODE::ANALOG}}),
            RLYB_FB3, ({ADC1_BASE, 11, {{GPIOC_BASE, 1}, GPIODriver::MODE::ANALOG}}),
        );
};