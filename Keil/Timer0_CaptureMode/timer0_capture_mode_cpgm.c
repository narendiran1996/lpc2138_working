#include<LPC213x.h>

#define X_CPU 10000000L
#define C_CLK 60000000L // since multiplier in PLL0 is 4
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

void timer0_counter_mode_Counton_falling_CAP0_2()
{
	// reset timer counter(T0TC) and prescale counter(T0PC) by setting counter reset in Timer Control Register - bit 1 of T0TCR
	T0TCR |= (1<<1);
	// let's remove the reset bit
	T0TCR &= ~(1<<1);
	
	// setting mode as Counter Mode in Timer Count Control register by T0CTCR[1:0]
	// 01 - increment TC on rising edge
	// 10 - increment TC on falling edge
	// 11 - increment TC on both edge
	// lets' go for falling edge 
	T0CTCR |= (1<<1);
	T0CTCR &= ~(1<<0);
	
	// prescale value
	// by setting the value in Precalare register - T0PR
	// lets' divide the external even by NOTHING
	T0PR = 0;	
	
	// next on which capture pin should this happend
	// setting Count INput Select in Timer Count Control register by T0CTCR[3:2]
	// 10 - on CAP0.2 - P0.6
	T0CTCR |= (1<<3);
	T0CTCR &= ~(1<<2);
	
	// So selecting that function on P0.6 - 10 
	PINSEL0 |= (1<<13);
	PINSEL0 &= ~(1<<12);
	
	// enabling counting by setting Counter Enable in timer Control Register - bit 0 of T0TCR
	T0TCR |= (1<<0);
	
}
void timer0_counter_mode_Counton_both_CAP0_2()
{
	// reset timer counter(T0TC) and prescale counter(T0PC) by setting counter reset in Timer Control Register - bit 1 of T0TCR
	T0TCR |= (1<<1);
	// let's remove the reset bit
	T0TCR &= ~(1<<1);
	
	// setting mode as Counter Mode in Timer Count Control register by T0CTCR[1:0]
	// 01 - increment TC on rising edge
	// 10 - increment TC on falling edge
	// 11 - increment TC on both edge
	// lets' go for both edge 
	T0CTCR |= (1<<1);
	T0CTCR |= (1<<0);
	
	// prescale value
	// by setting the value in Precalare register - T0PR
	// lets' divide the external even by NOTHING
	T0PR = 0;	
	
	// next on which capture pin should this happend
	// setting Count INput Select in Timer Count Control register by T0CTCR[3:2]
	// 10 - on CAP0.2 - P0.6
	T0CTCR |= (1<<3);
	T0CTCR &= ~(1<<2);
	
	// So selecting that function on P0.6 - 10 
	PINSEL0 |= (1<<13);
	PINSEL0 &= ~(1<<12);
	
	// enabling counting by setting Counter Enable in timer Control Register - bit 0 of T0TCR
	T0TCR |= (1<<0);
	
}
void timer0_timer_mode_Captureon_falling_CAP0_1_CR1()
{
	// reset timer counter(T0TC) and prescale counter(T0PC) by setting counter reset in Timer Control Register - bit 1 of T0TCR
	T0TCR |= (1<<1);
	// let's remove the reset bit
	T0TCR &= ~(1<<1);
	
	// setting mode as Timer Mode in Timer Count Control register by T0CTCR[1:0] == 00
	T0CTCR &= ~(1<<1);
	T0CTCR &= ~(1<<0);
	
	// prescale value
	// by setting the value in Precalare register - T0PR
	// lets' divide the PCLK by 500
	T0PR = 500;	
	
	// let's confgiure configure control register by T0CCR
	// capture on falling on CAP0.1 - P0.4 TO CR1
	T0CCR |= (1<<4);
	
	// So selecting that function on P0.4 - 10 
	PINSEL0 |= (1<<9);
	PINSEL0 &= ~(1<<8);
	
	// enabling counting by setting Counter Enable in timer Control Register - bit 0 of T0TCR
	T0TCR |= (1<<0);
}
int main()
{
	PLL_clock_change();
	APB_Divder();
	
	// see the output by toggling P0.6 pin
	//timer0_counter_mode_Counton_falling_CAP0_2();	
	// timer0_counter_mode_Counton_both_CAP0_2();
	
	// see the CR1 register
	timer0_timer_mode_Captureon_falling_CAP0_1_CR1();

	while(1)
	{	

	}
	
}

