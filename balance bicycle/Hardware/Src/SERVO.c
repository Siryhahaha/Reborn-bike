#include <stdio.h>
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "SERVO.h"

void Servo_Init(void)
{
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1500); // 设置占空比（1500/20000 = 7.5%）
}

float Servo_GetAngle(void)
{
	float Compare = __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_1);
	float Angle = (Compare - 500) / 2000 * 180 - 90;

	return Angle;
}

void Servo_Set(float Angle)
{
	if (Angle > 90)
	{
		Angle = 90;
	}
	else if (Angle < -90)
	{
		Angle = -90;
	}
	float Compare = (Angle + 90) / 180 * 2000 + 500;		//500-2500
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, Compare);
}

void Servo_Left(void)
{
	Servo_Set(-30);
}

void Servo_Right(void)
{
	Servo_Set(30);
}

void Servo_TurnLeft(void)
{
	float Angle = Servo_GetAngle();
	Angle -= 15;
	Servo_Set(Angle);
}

void Servo_TurnRight(void)
{
	float Angle = Servo_GetAngle();
	Angle += 15;
	Servo_Set(Angle);
}

/*	
	float Angle = -90;

	while (1)
	{
		OLED_ShowFloat(1,1,Angle);
		Servo_Set(Angle);
		OLED_ShowFloat(2,1,Servo_GetAngle());
		HAL_Delay(500);
		Angle += 10;

	}



*/



