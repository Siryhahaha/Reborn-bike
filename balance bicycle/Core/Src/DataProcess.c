#include "stdio.h"
#include "string.h"
#include "stm32f1xx_hal.h"
#include "main.h"
#include "usart.h"
#include "OLED.h"
#include "PID.h"
#include "DataProcess.h"

void SendResponse(const char* fmt, float val)
{
    char buf[40];
    snprintf(buf, sizeof(buf), "@%s\r\n", fmt);
    HAL_UART_Transmit(&huart2, (uint8_t*)buf, strlen(buf), 10000);
}

void ProcessUARTCommand(char* cmd)
{
    if(strncmp(cmd, "Mode0", 5) == 0) 
    {
        // 运行模式初始化
        mode = 0;
        OLED_Clear();
        OLED_ShowString(3,1,"Power:  %%");

    }
    else if(strncmp(cmd, "Mode1", 5) == 0) 
    {
        // 调试模式初始化
        mode = 1;
        OLED_Clear();
        OLED_ShowString(1, 1, "Kp1:");
        OLED_ShowString(2, 1, "Kd1:");
        OLED_ShowString(3, 1, "Kp2:");
        OLED_ShowString(4, 1, "Ki2:");
        OLED_ShowFloat(1, 5, vertical_kp);
        OLED_ShowFloat(2, 5, vertical_kd);
        OLED_ShowFloat(3, 5, velocity_kp);
        OLED_ShowFloat(4, 5, velocity_ki);
    } 
    if (mode == 1)
    {
        if(strncmp(cmd, "Kp1:", 4) == 0) 
        {
            if(sscanf(cmd + 4, "%f", &vertical_kp) == 1)
                SendResponse("vertical_kp Set:%.2f", vertical_kp);
                OLED_ShowFloat(1, 5, vertical_kp);
        }
        else if(strncmp(cmd, "Kd1:", 4) == 0) 
        {
            if(sscanf(cmd + 4, "%f", &vertical_kd) == 1)
                SendResponse("vertical_kd Set:%.2f", vertical_kd);
                OLED_ShowFloat(2, 5, vertical_kd);
        }
        else if(strncmp(cmd, "Kp2:", 4) == 0) 
        {
            if(sscanf(cmd + 4, "%f", &velocity_kp) == 1)
                SendResponse("velocity_kp Set:%.2f", velocity_kp);
                OLED_ShowFloat(3, 5, velocity_kp);
        }
        else if(strncmp(cmd, "Ki2:", 4) == 0) 
        {
            if(sscanf(cmd + 4, "%f", &velocity_ki) == 1)
                SendResponse("velocity_ki Set:%.2f", velocity_ki);
                OLED_ShowFloat(4, 5, velocity_ki);
        }
    }
}
