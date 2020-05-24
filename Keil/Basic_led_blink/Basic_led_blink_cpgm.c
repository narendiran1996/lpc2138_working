#include<LPC213x.h>

#define X_CPU 10000000L
#define C_CLK 60000000L // since multiplier in PLL0 is 5
#define P_CLK 60000000L // since vpd_div is made 01 - repressing C_CLK


void PLL_clock_change()
{
	/* Enabling PLL Feed first */
	// PLLCON[0] = PLLE
	PLLCON  |= (1<<0);
	
	/* the F_OSC is 10MHz, so let's make the C_CLK to 60MHz */
	/* we know that CCLK = M x F_OSC */
	// so M would be 6,
	// to get M of 6, 5 should be written to MSEL
	// PLLCFG[4:0] = MSEL
	PLLCFG = (PLLCFG  & 0b11100000) | 5;
	
	/* we know that FCCO = CCLK x 2 x P */
	// to get P, by selecting F_CCO to min and max valu of current controlled oscillator(156 MHz to 320 MHz)
	// we sould get P to be 1.95 and 4 so let's select P to be 2
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
void APB_Divder()
{
	// making bit[1:0] as 01 to get same as CCLK
	VPBDIV = VPBDIV | (1<<0);
	VPBDIV = VPBDIV & ~(1<<1);
}
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


void square_wave_on_MAT0_1(unsigned int t_period_us)
{
	// minimimum time period = 1us
	// maximum time period = 
	// reset timer counter(T0TC) and prescale counter(T0PC) by setting counter reset in Timer Control Register - bit 1 of T0TCR
	T0TCR |= (1<<1);
	// let's remove the reset bit
	T0TCR &= ~(1<<1);
	
	
	// setting mode as Timer Mode in Timer Count Control register by T0CTCR[1:0] == 00
	T0CTCR &= ~(1<<1);
	T0CTCR &= ~(1<<0);
	
	// prescale value
	// by setting the value in Precalare register - T0PR
	// since the PCLK is 60MHz, 
	// let's count from 0 to 14 for 0.25 us counter
	// lets' divide the PCLK by 15
	T0PR = 14;	
	
	
	T0MR1 = (2 * t_period_us) - 1; // since 0.25us prescaling and it's time period

	// what should happend when match happens
	// let's reset the T0TC when match happends in MR1
	// by setting the bit 4 in Timer Match Control Register T0MCR
	T0MCR |= (1<<4);
	
	// selecting MR1 function for P0.5 = 10
	PINSEL0 |= (1<<11);
	PINSEL0 &= ~(1<<10);
	
	// toggle pin MAT0.1 on TC match with MR1 - P0.5
	// by setting T0EMR[7:6] -- 11
	T0EMR |= (1<<7);
	T0EMR |= (1<<6);
	
	// enabling counting by setting Counter Enable in timer Control Register - bit 0 of T0TCR
	T0TCR |= (1<<0);
}

int main()
{
	PLL_clock_change();
	APB_Divder();
	
	IODIR0 = IODIR0 | (1<<16);
	
	// see the Output at P0.5
	square_wave_on_MAT0_1(3);
	while(1)
	{
		IOSET0 = IOSET0 | (1<<16);
		custom_delay();
		IOCLR0 = IOCLR0 | (1<<16);
		custom_delay();
	}
}
