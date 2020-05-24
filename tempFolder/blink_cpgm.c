#include "stm32f10x.h"



#define LED_BLUE_GPIO GPIOC
#define LED_BLUE_PIN  9



int main(void)
{
	int i;
	
	RCC->APB2ENR |= (1<<4);
	
	GPIOC->CRH &= ~(0x0F<<20);
	GPIOC->CRH |= (1<<21);
	
	while(1)
	{
		GPIOC->BSRR = (1<<13);
		for(i=0;i<1000;i++);
		GPIOC->BRR = (1<<13);
		for(i=0;i<1000;i++);
	}
}
