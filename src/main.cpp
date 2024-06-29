#include "GPIODriver.h"
#include "util.h"

int main() {
    GPIODriver red_led({GPIOE_BASE, 1}, 
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


    red_led.configure();
    green_led.configure();
    sw1_led.configure();
    while(1){

    }
