#include "ResourceHandler.h"    
#include "UARTHAL.h"
#include "SYSTICK.h"
#include "BoardUART.h"
#include "BoardGPIO.h"
#include "util.h"
#include "Programming.h"

int main() {
    __disable_irq();
    
    SYSTICK::DriverInit();
    ResourceHandler::Init();
    Programming::Init();

    __enable_irq();

    // bool led_level = false; 
    BoardUART::GetUARTResource(BoardUART::UARTResource_t::DEBUG).puts("CLC connected.\n\r", 16);
    while(1){
        ResourceHandler::ShellTask();
        // Programming::Task();
    }
}


extern "C" {
    extern void USART3_IRQHandler();
    void USART3_IRQHandler() {
        static constexpr const UARTResource & Resource = BoardUART::GetUARTResource(BoardUART::UARTResource_t::DEBUG);
        Resource.IRQHandler();
    }
}