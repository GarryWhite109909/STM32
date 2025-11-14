#include "main.h"
#include "delay.h"

int main()
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitTypeDef g;
	g.GPIO_Mode = GPIO_Mode_OUT;
	g.GPIO_Speed = GPIO_Speed_50MHz;
	g.GPIO_OType = GPIO_OType_PP;
	
	g.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOF, &g);
	
	g.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOF, &g);
	
	g.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOE, &g);
	
	g.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOE, &g);
	
	GPIO_SetBits(GPIOF, GPIO_Pin_9);
	GPIO_SetBits(GPIOF, GPIO_Pin_10);
	GPIO_SetBits(GPIOE, GPIO_Pin_13);
	GPIO_SetBits(GPIOE, GPIO_Pin_14);
	while(1)
	{
		GPIO_ResetBits(GPIOE, GPIO_Pin_14);
		Mdelay_Lib(1000);
		GPIO_SetBits(GPIOE, GPIO_Pin_14);
		
		GPIO_ResetBits(GPIOE, GPIO_Pin_13);
		Mdelay_Lib(1000);
		GPIO_SetBits(GPIOE, GPIO_Pin_13);

		GPIO_ResetBits(GPIOF, GPIO_Pin_10);
		Mdelay_Lib(1000);
		GPIO_SetBits(GPIOF, GPIO_Pin_10);

		GPIO_ResetBits(GPIOF, GPIO_Pin_9);
		Mdelay_Lib(1000);
		GPIO_SetBits(GPIOF, GPIO_Pin_9);
	}
}
