#include<LPC213x.h>

#define F_OSC 12000000L
#define C_CLK 60000000L // since multiplier in PLL0 is 5
#define P_CLK 12000000L // since vpd_div is made 000 - repressing C_CLK/4 (default)
void custom_delay()
{
	int i,j;
	for(i=0;i<1234;i++)
	{
		for(j=0;j<123;j++)
		{
		}
	}
}
void PLL_clock_change()
{
	/* Enabling PLL Feed first */
	// PLLCON[0] = PLLE
	PLLCON  |= (1<<0);
	
	/* the F_OSC is 12MHz, so let's make the C_CLK to 60MHz */
	/* we know that CCLK = M x F_OSC */
	// so M would be 5,
	// to get M of 5, 4 should be written to MSEL
	// PLLCFG[4:0] = MSEL
	PLLCFG = (PLLCFG  & 0b11100000) | 4;
	
	/* we know that FCCO = CCLK x 2 x P */
	// to get P, by selecting F_CCO to min and max valu of current controlled oscillator(156 MHz to 320 MHz)
	// we sould get P to be 1.3 and 2.67 so let's select P to be 2
	// to get P value of 2, 01 should be wrtten to PSEL
	// PLLCFG[6:5] = PSEL	
	PLLCFG |= (1<<5);
	PLLCFG &= ~(1<<6);	


	//PLL feeding to make the change and lock it
	PLLFEED = 0XAA;
	PLLFEED = 0X55;
	
	/* chekcing the PLL LOCK status */
	// PLLSTAT[10] = PLOCK
	while((PLLSTAT & (1<<10)) == 0X00);
	
	/* Connecting PLL Feed after locked */
	// PLLCON[1] = PLLC
	PLLCON  |= (1<<1);
	
	//PLL feeding to make the change and lock it
	PLLFEED = 0XAA;
	PLLFEED = 0X55;

	
	
}
int main()
{
	
	PLL_clock_change();
	IODIR0 = IODIR0 | (1<<16);
	
	while(1)
	{
		IOSET0 = IOSET0 | (1<<16);
		custom_delay();
		IOCLR0 = IOCLR0 | (1<<16);
		custom_delay();
	}
	return 0;
}
