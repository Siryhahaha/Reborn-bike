#include "stm32f1xx_hal.h"
#include "tim.h"
#include "main.h"
#include "MOTOR.h"

#define abs(x) 			(x > 0 ? x : -x)

float motor_duty = 0;

void MOTOR_Init(void)
{
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
}

void MOTOR_SetDuty(float duty)
{
    motor_duty = duty;
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (1 - abs(duty)) * 7200);
}

void MOTOR_SpeedDelta(float d)
{
    motor_duty += d;
    if (motor_duty > 1) motor_duty = 1;
    else if (motor_duty < -1) motor_duty = -1;
    MOTOR_SetDir(-motor_duty);
    MOTOR_SetDuty(motor_duty);
}

/**
 * @brief 控制电机的正反转与停止
 * @param Dir 正数为正转，负数为反转，零为停止
 */
void MOTOR_SetDir(int Dir)
{
    if (Dir > 0)
    {
        HAL_GPIO_WritePin(MOTOR_POS_DIR_GPIO_Port, MOTOR_POS_DIR_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_NEG_DIR_GPIO_Port, MOTOR_NEG_DIR_Pin, GPIO_PIN_RESET);
    }
    else if (Dir < 0)
    {
        HAL_GPIO_WritePin(MOTOR_NEG_DIR_GPIO_Port, MOTOR_NEG_DIR_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_POS_DIR_GPIO_Port, MOTOR_POS_DIR_Pin, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(MOTOR_NEG_DIR_GPIO_Port, MOTOR_NEG_DIR_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(MOTOR_POS_DIR_GPIO_Port, MOTOR_POS_DIR_Pin, GPIO_PIN_SET);
    }
}
