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
void init_Timer0_TimerMode_basic()
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
	// lets' count the the PCLK as it is 
	T0PR = 0;	
	
	// enabling counting by setting Counter Enable in timer Control Register - bit 0 of T0TCR
	T0TCR |= (1<<0);
}

void init_Timer0_TimerMode_Match0_reset()
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
	
	// let the match value be 0x1234
	T0MR0 = 0x1234;

	// what should happend when match happens
	// let's reset the T0TC when match happends
	// by setting the bit 1 in Timer Match Control Register T0MCR
	T0MCR |= (1<<1);
	
	// enabling counting by setting Counter Enable in timer Control Register - bit 0 of T0TCR
	T0TCR |= (1<<0);
}

void init_Timer0_TimerMode_Match0_stop()
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
	
	// let the match value be 0x1234
	T0MR0 = 0x1234;

	// what should happend when match happens
	// let's  stop the T0TC when match happends
	// stop by setting the bit 2 in Timer Match Control Register T0MCR
	T0MCR |= (1<<2);
	
	// enabling counting by setting Counter Enable in timer Control Register - bit 0 of T0TCR
	T0TCR |= (1<<0);
}

__irq void my_timer0_irq(void);
void init_Timer0_TimerMode_Match0_reset_and_interrupt()
{
	// reset timer counter(T0TC) and prescale counter(T0PC) by setting counter reset in Timer Control Register - bit 1 of T0TCR
	T0TCR |= (1<<1);

	
	
	// setting mode as Timer Mode in Timer Count Control register by T0CTCR[1:0] == 00
	T0CTCR &= ~(1<<1);
	T0CTCR &= ~(1<<0);
	
	// prescale value
	// by setting the value in Precalare register - T0PR
	// lets' divide the PCLK by 500
	T0PR = 5000;	
	
	// let the match value be 0x1234
	T0MR0 = 0x1234;

	// what should happend when match happens
	// let's reset and interrupt on match the T0TC when match happends
	// restet by setting the bit 1 in Timer Match Control Register T0MCR
	T0MCR |= (1<<1);
	// interrupt by setting bit 0  in Timer Match Control Register T0MCR
	T0MCR |= (1<<0);

	//VICVectAddr = (unsigned int)my_timer0_irq;
	
	// let's give the address for ISR of slot 8
	VICVectAddr8 = (unsigned int)my_timer0_irq;

	// lts' assign TIMER0 IRQ to slot 8
	VICVectCntl8 = (1<<5) | 4; // enabling and assign TIMER0 (interrupt number 4)
	
	// let's select IRQ for TIMER0 by clearing bit 4  in Interrupt Select Register
	VICIntSelect &= ~(1<<4);
	
	// enable timer 0 interrupt by setting bit 4 in Interrupt Enable register
	VICIntEnable |= (1<<4);
	
	// let's remove the reset bit
	T0TCR &= ~(1<<1);
	// enabling counting by setting Counter Enable in timer Control Register - bit 0 of T0TCR
	T0TCR |= (1<<0);
}

void init_Timer0_TimerMode_Match0_reset_ExternalMatch_toggle_MAT0_0()
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
	// since the PCLK is 20MHz, 
	// let's count from 0 to 19 for 1 us counter
	// lets' divide the PCLK by 20
	T0PR = 19;	
	
	
	// let's have a on-time of 5us and off-time of 5us
	// so we count from 0 to 4 for 5us
	// now, we can have 10us square wave
	// so we get match value of 4
	T0MR0 = 0x4;

	// what should happend when match happens
	// let's reset the T0TC when match happends
	// by setting the bit 1 in Timer Match Control Register T0MCR
	T0MCR |= (1<<1);
	
	// selecting MR0 function for P0.3  = 10
	PINSEL0 |= (1<<7);
	PINSEL0 &= ~(1<<6);
	
	// toggle pin MAT0.0 on TC match with MR0 - P0.3
	// by setting T0EMR[5:4] -- 11
	T0EMR |= (1<<5);
	T0EMR |= (1<<4);
	
	// enabling counting by setting Counter Enable in timer Control Register - bit 0 of T0TCR
	T0TCR |= (1<<0);
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
	
	
	T0MR1 = (4 * t_period_us) - 1; // since 0.25us prescaling and it's time period

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


void delay_us(unsigned int us_delay)
{
	// reset timer counter(T0TC) and prescale counter(T0PC) by setting counter reset in Timer Control Register - bit 1 of T0TCR
	T0TCR |= (1<<1);
	// let's remove the reset bit
	T0TCR &= ~(1<<1);
	
	
	// setting mode as Timer Mode in Timer Count Control register by T0CTCR[1:0] == 00
	T0CTCR &= ~(1<<1);
	T0CTCR &= ~(1<<0);
	
	
	T0PR = 14;
	
	T0MR0 = (3*(us_delay));

	// what should happend when match happens
	// let's  stop the T0TC when match happends
	// stop by setting the bit 2 in Timer Match Control Register T0MCR
	T0MCR |= (1<<2);	
	T0MCR |= (1<<0);
	
	
	// enabling counting by setting Counter Enable in timer Control Register - bit 0 of T0TCR
	T0TCR |= (1<<0);
	
	while((T0IR & (1<<0)) == 0X00)	{}
		
	T0TCR &= ~(1<<0);
	T0TCR &= ~(1<<1);
	T0TC = 0;
	T0IR |= (1<<0);
	
}

int main()
{
	PLL_clock_change();
	APB_Divder();
	IODIR0 |= (1<<0);
	IOSET0 |= (1<<0);
	
	// can see the output in register T0TC location
	// init_Timer0_TimerMode_basic();
	
	// can see the output in register T0TC location
	//init_Timer0_TimerMode_Match0_reset();
	
	// can see the output in register T0TC location
	//init_Timer0_TimerMode_Match0_stop();
	
	// dont' know why it doesn't work
	// init_Timer0_TimerMode_Match0_reset_and_interrupt();
	
	// OUTPUT at P0.3
	// init_Timer0_TimerMode_Match0_reset_ExternalMatch_toggle_MAT0_0();
	
	// see the Output at P0.5
	// square_wave_on_MAT0_1(1);
	
		
	
	while(1)
	{	
		// dont' know why - delay in less time doesn't seem to work
//		IOSET0 |= (1<<0);
//		delay_us(120);		
//		IOCLR0 |= (1<<0);	
//		delay_us(120);		
	}
	
}


__irq void my_timer0_irq(void)
{

	IOSET0 |= (1<<0);
	
	// clear the interrup for match channel 0 for further interrupt in Timer Interrupt Register
	T0IR |= (1<<0);

	// set address of VICVectAddr to further interrupt
	VICVectAddr = 0x00;
}

