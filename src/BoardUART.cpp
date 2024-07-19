#include "BoardUART.h"  
#include "UARTDriver.h"

void BoardUART::Init() {
    BoardUART::GetUARTResource(BoardUART::UARTResource_t::DEBUG).configure(BoardUART::GetUARTConfig(BoardUART::UARTConfig_t::DEBUG));
}