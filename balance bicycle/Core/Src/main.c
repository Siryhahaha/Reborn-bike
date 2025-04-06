/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OLED.h"
#include "SERVO.h"
#include "MOTOR.h"
#include "WHEEL.h"
#include "ENCODER.h"
#include "MPU6050.h"
#include "DataProcess.h"
#include "PID.h"
#include "vofa.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define abs(x) 			(x > 0 ? x : -x)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t mode = 0;
uint8_t rx_data;
float pitch, roll, yaw;
short gyro[3], accel[3];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//蓝牙中断接收函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 
{
  static uint8_t tmp_data;  //临时存储
  
  if (huart->Instance == USART2) 
  {
    tmp_data = rx_data;  //获取数据
    
    switch(uart2_rx_state) 
    {
      case 0:
        if(tmp_data == '@') 
        {
          uart2_rx_state = 1;
          uart2_rx_length = 0;
          uart2_rx_flag = 0;
        }
        break;
        
      case 1:  //接收数据体
        if(tmp_data == '\r') 
        {
          uart2_rx_state = 2;
        } 
        else 
        {
          if(uart2_rx_length < RX_BUFFER_SIZE-1) 
          { 
            uart2_rx_buffer[uart2_rx_length++] = tmp_data;
          }
        }
        break;
        
      case 2:
        if(tmp_data == '\n') 
        {
          uart2_rx_buffer[uart2_rx_length] = '\0';
          uart2_rx_flag = 1;
        }
        uart2_rx_state = 0;  //重置
        break;
    }

    HAL_UART_Receive_IT(&huart2, &rx_data, 1);
  }
}

//MPU6050外部中断回调
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == MPU6050_EXTI_Pin)
  {
    int vertical_out = Vertical(roll, gyro[0]),
        velocity_out = Velocity(Encoder_Get());
    
    int Out = vertical_out + velocity_out;
    Wheel_Load(Out);

    VOFA_Transmit(roll);
  }
}

float ADC_GetPower(ADC_HandleTypeDef *hadc, uint8_t times)
{
  int total_value = 0;
  float avg_value = 0;
  for (uint8_t i = 0; i < times; ++i)
  {
    total_value += HAL_ADC_GetValue(hadc);
    HAL_Delay(5);
  }
  avg_value = (float)total_value / times;
  return (avg_value / 4096 * 3.3 * 11.582 - 11.7) / (12.8 - 11.7) * 100;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();
  Servo_Init();
  MOTOR_Init();
  Wheel_Init();
  int ret;
  do
  {
    ret = MPU6050_DMP_Init();
    HAL_Delay(1000);
  } while (ret);
  HAL_UART_Receive_IT(&huart2, &rx_data, 1);
  HAL_ADC_Start(&hadc1);

  OLED_ShowString(3, 1, "Power:     %");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    if (mode == 0)
    {
      OLED_ShowFloat(3, 7, ADC_GetPower(&hadc1, 5));
    }
    /*蓝牙代码*/
    if(uart2_rx_flag) 
    {
        uart2_rx_flag = 0;  // 清除标志
        ProcessUARTCommand(uart2_rx_buffer);  // 处理接收到的命令
    }
    /*以上是蓝牙代码*/

    /*以下是mpu6050代码*/
    MPU6050_DMP_Get_Data(&pitch, &roll, &yaw);
    MPU6050_Get_Gyroscope(gyro);
    MPU6050_Get_Accelerometer(accel);
    /*以上是mpu6050代码*/

    if (abs(roll - med_angle) < 15)
    {
      Wheel_SetDir(0);
      wheel_en = 0;
    }
    else
      wheel_en = 1;
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
