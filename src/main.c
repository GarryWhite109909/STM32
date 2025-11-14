#include "main.h"
#include "delay.h"

int main()
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_InitTypeDef g;
	g.GPIO_Pin = GPIO_Pin_9;
	g.GPIO_Mode = GPIO_Mode_OUT;
	g.GPIO_Speed = GPIO_Speed_50MHz;
	g.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOF, &g);
	
	GPIO_ResetBits(GPIOF, GPIO_Pin_9);
	
	while(1);
}
