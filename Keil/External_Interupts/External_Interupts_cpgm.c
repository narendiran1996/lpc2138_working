//#include<LPC213x.h>

//#define X_CPU 12000000L
//#define C_CLK 12000000L // since multiplier in PLL0 is 1


//void init_EINT0_as_FIQ()
//{
//	// mkaing P0.1 to use EINT0 function
//	// PINSEL0[3:2] -- 11
//	PINSEL0 |= (1<<3) | (1<<2);
//	
//	
//	// make EINT0 as edge triggerd
//	// EXTMODE[0] - EXTMODE0
//	// 0- level tiggered, 1- Edge triggered
//	EXTMODE |= (1<<0);
//	
//	// make EINT0 as rising edge triggnred
//	// EXTPOLAR[0] - EXTPOLAR0
//	// 0 - rising, 1- Falling
//	EXTPOLAR |= (1<<0);

//	
//	// making EINT0 as FIQ
//	// by selecting FIQ or IRQ in VICIntSelect
//	// 0 - IRQ, 1 - FIQ
//	// VICIntSelect[14] - EINT0
//	VICIntSelect |= (1<<14);
//	
//	
//	// enabling EINT0 interrupt
//	// by setting in VICIntEnable
//	// VICIntSelect[14] - EINT0
//	VICIntEnable |= (1<<14);
//		
//		
//	// enabling the intrrupt
//	// EXTINT[0] - EINT0
//	EXTINT &= ~(1<<0);
//}


//__irq void my_irq_EINT1(void);
//void init_EINT1_as_IRQ()
//{
//	// mkaing P0.3 to use EINT1 function
//	// PINSEL0[7:6] -- 11
//	PINSEL0 |= (1<<7) | (1<<6);
//	
//	
//	// make EINT1 as edge triggerd
//	// EXTMODE[1] - EXTMODE1
//	// 0- level tiggered, 1- Edge triggered
//	EXTMODE &= ~(1<<1);
//	
//	// make EINT1 as rising edge triggnred
//	// EXTPOLAR[1] - EXTPOLAR1
//	// 0 - rising, 1- Falling
//	EXTPOLAR |= (1<<1);

//	
//	// making EINT1 as IRQ
//	// by selecting FIQ or IRQ in VICIntSelect
//	// 0 - IRQ, 1 - FIQ
//	// VICIntSelect[15] - EINT1
//	VICIntSelect &= ~(1<<15);
//	
//	// assigning address for ISR in VICVectAddr3 for slot 3
//	VICVectAddr15 = (unsigned int)my_irq_EINT1;
//	
//	// assinging slot 3 TO EINT1
//	// by configuring VICvectCntl3,
//	// VICvectCntl3[5] should be aenabled and VICvectCntl3[4:0] loaded with 3	
//	VICVectCntl15 = (1<<5) | 15;
//	

//	
//	// enabling EINT1 interrupt
//	// by setting in VICIntEnable
//	// VICIntSelect[15] - EINT1
//	VICIntEnable |= (1<<15);
//		
//		
//	// enabling the intrrupt
//	// EXTINT[1] - EINT1
//	EXTINT |= (1<<1);
//}

//int main()
//{
//	// Since EINT0 is at P0.1 we make it input
//	IODIR0 = IODIR0 & ~(1<<1);	
//	
//	// Since EINT1 is at P0.3 we make it input
//	IODIR0 = IODIR0 & ~(1<<3);
//	
//	// FOR led
//	IODIR0 |= (1<<16);
//	IOCLR0 |= (1<<16);
//	
//	
//	// don't know how to give isr	
//	//init_EINT0_as_FIQ();
//	
//	init_EINT1_as_IRQ();
//	
//	
//	while(1)
//	{
//		
//	}
//}
//void my_irq_EINT1(void) __irq
//{
//	if((IOPIN0 & (1<<16)) == 0)
//		IOSET0 |= (1<<16);
//	else
//		IOCLR0 |= (1<<16);
//	
//	// clearing the intrrupt for further interrupt
//	// EXTINT[1] - EINT1
//	EXTINT |= (1<<1);
//	
//	VICVectAddr = 0x0;
//}
#include <LPC214X.H>
void FIQ_Handler()__irq	// ISR for FIQ
{
}
int main()
{
	
}