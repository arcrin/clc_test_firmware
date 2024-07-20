#include "BoardUART.h"
#include "UARTResource.h"


// extern "C" {
//     extern void USART3_IRQHandler(void);
//     void USART3_IRQHandler(void) {
//         static constexpr const UARTResource & Resource = BoardUART::GetUARTResource(BoardUART::UARTResource_t::DEBUG);
//         Resource.IRQHandler();
//     }
// }