#include "stm32f1xx_hal.h"
#include "tim.h"
#include "WHEEL.h"
#include "vofa.h"

#define abs(x) 			(x > 0 ? x : -x)

int PWM_MAX = 7000,
	PWM_MIN = -7000;

void Wheel_Init(void)
{
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_GPIO_WritePin(WHEEL_EN_GPIO_Port, WHEEL_EN_Pin, GPIO_PIN_SET);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
}

/**
 * @brief 动量轮的正反转与停止
 * @param Dir 正数为正转，负数为反转，零为停止
 */
void Wheel_SetDir(int Dir)
{
	if (Dir > 0)
	{
		HAL_GPIO_WritePin(WHEEL_DIR_GPIO_Port, WHEEL_DIR_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(WHEEL_EN_GPIO_Port, WHEEL_EN_Pin, GPIO_PIN_SET);
	}
	else if (Dir < 0)
	{
		HAL_GPIO_WritePin(WHEEL_DIR_GPIO_Port, WHEEL_DIR_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(WHEEL_EN_GPIO_Port, WHEEL_EN_Pin, GPIO_PIN_SET);
	}
	else
		HAL_GPIO_WritePin(WHEEL_EN_GPIO_Port, WHEEL_EN_Pin, GPIO_PIN_RESET);
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
	Wheel_SetDir(PWM);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, abs(PWM));
}
