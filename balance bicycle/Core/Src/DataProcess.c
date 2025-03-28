#include "stm32f1xx_hal.h"
#include "DataProcess.h"
#include "usart.h"
#include "OLED.h"
#include "stdio.h"
#include "string.h"

/*为了方便我先定义在这里，你按需要换到别的地方比如control。c*/
uint8_t Mode = 0;
float Kp1 = 0;


void ProcessUARTCommand(char* cmd)
{
    if(strncmp(cmd, "Mode0", 5) == 0) 
    {
        // 运行模式初始化
        Mode = 0;
        OLED_Clear();
        OLED_ShowString(3,1,"Power:  %");

    }
    else if(strncmp(cmd, "Mode1", 5) == 0) 
    {
        // 调试模式初始化
        Mode = 1;
        OLED_Clear();
        OLED_ShowString(1,1,"Kp1:");
    } 
    if (Mode == 1)
    {
        if(strncmp(cmd, "Kp1:", 4) == 0) 
        {
            if(sscanf(cmd+4, "%f", &Kp1) == 1)
                SendResponse("Kp Set:%.2f", Kp1);
                OLED_ShowFloat(1,5,Kp1);
        }
    }

}

void SendResponse(const char* fmt, float val)
{
    char buf[40];
    snprintf(buf, sizeof(buf), "@%s\r\n", fmt);
    HAL_UART_Transmit_IT(&huart2, (uint8_t*)buf, strlen(buf));
}
