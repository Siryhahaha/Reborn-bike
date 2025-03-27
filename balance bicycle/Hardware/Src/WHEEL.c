#include "stm32f1xx_hal.h"
#include "WHEEL.h"
#include "stdlib.h"
#include "tim.h"
//还没测试的哈

uint8_t Wheel_EN = 0;
uint8_t Wheel_DIR = 0;

int PWM_MAX = 10000;
int PWM_MIN = -10000;

extern int Roll;

void Limit(int *PWM)
{
	if(*PWM>PWM_MAX)*PWM=PWM_MAX;
	if(*PWM<PWM_MIN)*PWM=PWM_MIN;
}

void Load(int PWM)
{
	if(PWM>0)   Wheel_EN=1,Wheel_DIR=1;
	else        Wheel_EN=1,Wheel_DIR=0;
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, abs(PWM));
}

void Stop_Detect(void)
{   
	if(abs(Roll - 0)>12)
    {
        // Beep=1;
        Wheel_EN = 0;
    }
    else 
	{
		// Beep=0;
		Wheel_EN = 1;
	}
}