#include "main.h"
#include "delay.h"

int main()
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef g;
	g.GPIO_Pin = GPIO_Pin_9;
	g.GPIO_Mode = GPIO_Mode_OUT;
	g.GPIO_Speed = GPIO_Speed_50MHz;
	g.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOF, &g);
	
	g.GPIO_Pin = GPIO_Pin_0;
	g.GPIO_Mode = GPIO_Mode_IN;
	g.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &g);
	
	GPIO_SetBits(GPIOF, GPIO_Pin_9);
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
		{
			if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_9) == 0)
			{
				GPIO_SetBits(GPIOF, GPIO_Pin_9);
			}
			else
			{
				GPIO_ResetBits(GPIOF, GPIO_Pin_9);
			}
			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0);
		}
	}
//	while(1)
//	{
//		GPIO_ResetBits(GPIOE, GPIO_Pin_14);
//		Mdelay_Lib(1000);
//		GPIO_SetBits(GPIOE, GPIO_Pin_14);
		
//		GPIO_ResetBits(GPIOE, GPIO_Pin_13);
//		Mdelay_Lib(1000);
//		GPIO_SetBits(GPIOE, GPIO_Pin_13);

//		GPIO_ResetBits(GPIOF, GPIO_Pin_10);
//		Mdelay_Lib(1000);
//		GPIO_SetBits(GPIOF, GPIO_Pin_10);

//		GPIO_ResetBits(GPIOF, GPIO_Pin_9);
//		Mdelay_Lib(1000);
//		GPIO_SetBits(GPIOF, GPIO_Pin_9);
//	}
}
