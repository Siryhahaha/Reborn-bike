#include "usart.h"
#include "vofa.h"

unsigned char tail[4] = {    0x00, 0x00, 0x80, 0x7f  };

void VOFA_Transmit(float data)
{
    HAL_UART_Transmit(&huart2, (unsigned char *)&data, sizeof(float), 10000);
    HAL_UART_Transmit(&huart2, tail, 4, 10000);
}
