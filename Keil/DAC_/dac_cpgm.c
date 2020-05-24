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
void set_DAC()
{
	// seelcting functionallity for P0.25 as AOUT 
	// PINSEL1[19:18] = 10
	PINSEL0 |= (1<<19);
	PINSEL0 &= ~(1<<18);
	
	// setting the settingling time to 1us
	// by clearing the BIAS bit - DACR[16]	
	DACR &= ~(1<<16);
	
	int values = 0x3FF;
	// giving the value to Values - DACR[15:6]
	DACR = (DACR & 0xFFC0) | (values << 6);
	
}
int main()
{
	PLL_clock_change();
	APB_Divder();
	set_DAC();
	while(1)
	{
		
		
	}
}
