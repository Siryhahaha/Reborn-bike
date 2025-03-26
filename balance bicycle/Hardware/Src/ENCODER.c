#include "stm32f1xx_hal.h"
#include "ENCODER.h"
#include "tim.h"

int16_t Encoder_Get(void)
{
    int16_t Temp;
    Temp = (int16_t)(__HAL_TIM_GET_COUNTER(&htim3));
    __HAL_TIM_SET_COUNTER(&htim3,0);
    
    if (Temp > 60000)
    {
        Temp = Temp - 65536;
    }

    return Temp;
}
