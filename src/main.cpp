#include "GPIODriver.h"
#include "util.h"

int main() {
    GPIODriver red_led({GPIOE_BASE, 0}, 
                    GPIODriver::MODE::OUTPUT, 
                    0, 
                    false, 
                    false, 
                    GPIODriver::TYPE::PUSH_PULL, 
                    GPIODriver::SPEED::MEDIUM, 
                    GPIODriver::PULL::NONE);
    
    GPIODriver green_led({GPIOE_BASE, 1},
                    GPIODriver::MODE::OUTPUT, 
                    0, 
                    false, 
                    false, 
                    GPIODriver::TYPE::PUSH_PULL, 
                    GPIODriver::SPEED::MEDIUM, 
                    GPIODriver::PULL::NONE);

    GPIODriver sw1_led({GPIOE_BASE, 8},
                    GPIODriver::MODE::OUTPUT, 
                    0, 
                    false, 
                    false, 
                    GPIODriver::TYPE::PUSH_PULL, 
                    GPIODriver::SPEED::MEDIUM, 
                    GPIODriver::PULL::NONE);



    red_led.DriverInit();
    red_led.configure();
    red_led.set_level(false);

    green_led.DriverInit();
    green_led.configure();
    green_led.set_level(false);

    sw1_led.DriverInit();   
    sw1_led.configure();
    // sw1_led.set_level(false);
    bool sw1_led_state = false;
    sw1_led.set_level(sw1_led_state);
    while(1){
        _delay_ms(500);
        sw1_led_state = !sw1_led_state;
        sw1_led.set_level(sw1_led_state);
    }
}