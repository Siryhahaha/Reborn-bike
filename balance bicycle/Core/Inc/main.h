/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SERVO_PWM_Pin GPIO_PIN_0
#define SERVO_PWM_GPIO_Port GPIOA
#define BATTERY_VOLT_Pin GPIO_PIN_1
#define BATTERY_VOLT_GPIO_Port GPIOA
#define BLE_TX_Pin GPIO_PIN_2
#define BLE_TX_GPIO_Port GPIOA
#define BLE_RX_Pin GPIO_PIN_3
#define BLE_RX_GPIO_Port GPIOA
#define MOTOR_NEG_DIR_Pin GPIO_PIN_4
#define MOTOR_NEG_DIR_GPIO_Port GPIOA
#define MOTOR_POS_DIR_Pin GPIO_PIN_5
#define MOTOR_POS_DIR_GPIO_Port GPIOA
#define WHEEL_A_Pin GPIO_PIN_6
#define WHEEL_A_GPIO_Port GPIOA
#define WHEEL_B_Pin GPIO_PIN_7
#define WHEEL_B_GPIO_Port GPIOA
#define OLED_SCL_Pin GPIO_PIN_10
#define OLED_SCL_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_11
#define OLED_SDA_GPIO_Port GPIOB
#define WHEEL_PWM_Pin GPIO_PIN_8
#define WHEEL_PWM_GPIO_Port GPIOA
#define MOTOR_PWM_Pin GPIO_PIN_9
#define MOTOR_PWM_GPIO_Port GPIOA
#define MPU6050_EXTI_Pin GPIO_PIN_5
#define MPU6050_EXTI_GPIO_Port GPIOB
#define MPU6050_EXTI_EXTI_IRQn EXTI9_5_IRQn
#define MPU6050_SCL_Pin GPIO_PIN_6
#define MPU6050_SCL_GPIO_Port GPIOB
#define MPU6050_SDA_Pin GPIO_PIN_7
#define MPU6050_SDA_GPIO_Port GPIOB
#define WHEEL_EN_Pin GPIO_PIN_8
#define WHEEL_EN_GPIO_Port GPIOB
#define WHEEL_DIR_Pin GPIO_PIN_9
#define WHEEL_DIR_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
