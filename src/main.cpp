#include "ResourceHandler.h"    
#include "UARTHAL.h"
#include "BoardUART.h"
#include "BoardGPIO.h"
#include "util.h"

int main() {
    __disable_irq();

    ResourceHandler::Init();

    __enable_irq();

    
    BoardUART::GetUARTResource(BoardUART::UARTResource_t::DEBUG).puts("CLC connected.\n\r", 16);
    while(1){

        ResourceHandler::ShellTask();
    }
}


extern "C" {
    extern void USART3_IRQHandler();
    void USART3_IRQHandler() {
        static constexpr const UARTResource & Resource = BoardUART::GetUARTResource(BoardUART::UARTResource_t::DEBUG);
        Resource.IRQHandler();
    }
}