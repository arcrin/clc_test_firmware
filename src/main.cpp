#include "ResourceHandler.h"    
#include "UARTHAL.h"
#include "SYSTICK.h"
#include "Programming.h"




int main() {
    __disable_irq();
    // Enable HSE (High Speed External) clock
    RCC->CR |= RCC_CR_HSEON;

    // wait until HSE is ready
    while(!(RCC->CR & RCC_CR_HSERDY));

    // Configure Flash latency
    FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_1;

    // Configure the PLL
    // PLL source is HSE, PLL multiplier is 9 (16MHz / 2 * 9 = 72 MHz)
    RCC->CFGR |= RCC_CFGR_PLLSRC; // Set PLL source to HSE
    RCC->CFGR |= RCC_CFGR_PLLXTPRE; // HSE pre-divider for PLL entry clock
    RCC->CFGR |= RCC_CFGR_PLLMULL9; // Set PLL multiplier to 9

    // Enable the PLL
    RCC->CR |= RCC_CR_PLLON;

    // Wait until the PLL is ready
    while(!(RCC->CR & RCC_CR_PLLRDY));

    // Configure AHB, APB1, APB2 prescalers
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1; // AHB prescaler = 1   
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; // APB1 prescaler = 2
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1; // APB2 prescaler = 1

    // Set the ADC prescaler to 6 (12 MHz ADC clock)
    RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;
    // Select the PLL as the system clock source
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    // Wait until the PLL is used as the system clock source
    while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
    SYSTICK::DriverInit();
    ResourceHandler::Init();
    Programming::Init();

    __enable_irq();

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
