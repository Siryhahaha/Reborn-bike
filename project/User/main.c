#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "KEY.h"
#include "OLED.h"

uint8_t KeyNum;

int main(void)
{
	LED_Init();
//	Key_Init();
	OLED_Init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOD, GPIO_Pin_2 | GPIO_Pin_1);
	while (1)
	{
		OLED_ShowString(1,1,"HAPPY");
//		OLED_ShowString(1,11,"RICH");
//		OLED_ShowString(2,3,"STRONG");
//		OLED_ShowString(4,1,"BEAUTY");
//		OLED_ShowString(3,5,"MORE MONEY!");
	}
}
