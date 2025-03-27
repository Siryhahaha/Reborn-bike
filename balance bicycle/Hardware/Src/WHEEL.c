#include "stm32f1xx_hal.h"
#include "WHEEL.h"
#include "stdlib.h"
#include "tim.h"
//还没测试的哈

uint8_t Wheel_EN = 1;
uint8_t Wheel_DIR = 0;

int PWM_MAX = 7000;
int PWM_MIN = -7000;

// extern int Roll;
int Roll = 0;

void Wheel_Init(void)
{
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 7200);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}

int Wheel_Limit(int PWM)
{
	if(PWM > PWM_MAX)
	{
		return PWM_MAX;
	}
	else if(PWM < PWM_MIN)
	{
		return PWM_MIN;
	}
	return PWM;
}

void Wheel_Load(int PWM)
{
	PWM = Wheel_Limit(PWM);
	if (Wheel_EN == 1)
	{
		if(PWM>0)
		{
			Wheel_DIR=1;
		}
		else
		{
			Wheel_DIR=0;
		}       
		int compare = 7200 - 1 - abs(PWM);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, compare);
		// __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, abs(PWM));
		//此处是channel2也就是pa9就可以
	}
	else if (Wheel_EN == 0)
	{
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
	}

}

void Wheel_Stop_Detect(void)
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
