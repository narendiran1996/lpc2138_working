#include<LPC213x.h>

#define X_CPU 10000000L
#define C_CLK 60000000L // since multiplier in PLL0 is 6
#define P_CLK 60000000L // since vpd_div is made 01 - repressing C_CLK

void PLL_clock_change()
{
	/* Enabling PLL Feed first */
	// PLLCON[0] = PLLE
	PLLCON  |= (1<<0);
	
	/* the F_OSC is 10MHz, so let's make the C_CLK to 60MHz */
	/* we know that CCLK = M x F_OSC */
	// so M would be 6,
	// to get M of 5, 5 should be written to MSEL
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

void PWM_single_edge_channel_ONE()
{
	// select the functionality for PWM1 - P0.0 == 10
	PINSEL0 |= (1<<1);
	PINSEL0 &= ~(1<<0);
	
	// reset the PWM Timer Counter and PWM Prescale Counter
	// PWMTCR[1] -- Counter Reset
	PWMTCR |= (1<<1);

	
	// Prescalre value for the counter
	// PWMPR - divide the PCLK 
	// here we use 500
	PWMPR = 500;
	
	/* for sinlge edge and channel 1, we use set by Match Reg0 and reset by Match Reg1 */
	// so we set the needed values
	// PWMMR0 > PWMMR1 for correct operation
	PWMMR0 = 1000;
	
	PWMMR1 = 200;
	
	/* Now, we give what should happen when PWMTC reaches PWMMR0 and PWMMR1 */
	// when PWMTC reaches PWMMR0, PWMTC should reset back to 0 for further PWM
	// we do this by setting PWMMR0R - PWMMCR[1]
	PWMMCR |= (1<<1);
	
	// when PWMTC reaches PWMMR1, nothing should be done on PWMTC so we leave it now
	
	/* Selecting the edge and enabling the output */
	// Since, Channel ONE is always single edge we don't need to choose edge
	
	// Enabling the channel ONE is done by
	// PWMENA1 - PWMPCR[9]
	PWMPCR |= (1<<9);
	
	// enabling the PWM Mode
	// PWMTCR[3] -- PWM Enable
	PWMTCR |= (1<<3);
	
	// removing the Counter Reset for operation
	// PWMTCR[1] -- Counter Reset
	PWMTCR &= ~(1<<1);
	
	// enabling the counter to star the counter
	// PWMTCR[0] -- Counter Enable
	PWMTCR |= (1<<0);
	
}


void PWM_single_edge_channel_TWO_THREE()
{
	// select the functionality for PWM2 - P0.7 == 10
	PINSEL0 |= (1<<15);
	PINSEL0 &= ~(1<<14);
	
	// select the functionality for PWM3 - P0.1 == 10
	PINSEL0 |= (1<<3);
	PINSEL0 &= ~(1<<2);
	
	// reset the PWM Timer Counter and PWM Prescale Counter
	// PWMTCR[1] -- Counter Reset
	PWMTCR |= (1<<1);

	
	// Prescalre value for the counter
	// PWMPR - divide the PCLK 
	// here we use 500
	PWMPR = 500;
	
	/* for sinlge edge and channel 2, we use set by Match Reg0 and reset by Match Reg 2*/
	// so we set the needed values
	// PWMMR0 > PWMMR2 for correct operation
	PWMMR0 = 1000;
	
	PWMMR2 = 845;
	
	/* for sinlge edge and channel 3, we use set by Match Reg0 and reset by Match Reg 3*/
	// so we set the needed values
	// PWMMR0 > PWMMR3 for correct operation
	PWMMR0 = 1000;
	
	PWMMR3 = 550;
	
	
	/* Now, we give what should happen when PWMTC reaches PWMMR0 and PWMMR2 */
	// when PWMTC reaches PWMMR0, PWMTC should reset back to 0 for further PWM
	// we do this by setting PWMMR0R - PWMMCR[1]
	PWMMCR |= (1<<1);
	
	// when PWMTC reaches PWMMR2, nothing should be done on PWMTC so we leave it now
	
	// when PWMTC reaches PWMMR3, nothing should be done on PWMTC so we leave it now
	
	/* Selecting the edge and enabling the output */
	// selecting single edge for channel 2 by clearing PWMSEL2 - PWMPCR[2]
	PWMPCR &= ~(1<<2);
	// selecting single edge for channel 3 by clearing PWMSEL3 - PWMPCR[3]
	PWMPCR &= ~(1<<3);
	
	// Enabling the channel TWO is done by
	// PWMENA2 - PWMPCR[10]
	PWMPCR |= (1<<10);
	// Enabling the channel THREE is done by
	// PWMENA2 - PWMPCR[11]
	PWMPCR |= (1<<11);
	
	// enabling the PWM Mode
	// PWMTCR[3] -- PWM Enable
	PWMTCR |= (1<<3);
	
	// removing the Counter Reset for operation
	// PWMTCR[1] -- Counter Reset
	PWMTCR &= ~(1<<1);
	
	// enabling the counter to star the counter
	// PWMTCR[0] -- Counter Enable
	PWMTCR |= (1<<0);
	
}


void PWM_single_edge_generatePWM_channel_ONE(unsigned int onTimeus, unsigned int offTimeus)
{
	
	unsigned int totalTimeus = onTimeus + offTimeus;
	
	// select the functionality for PWM1 - P0.0 == 10
	PINSEL0 |= (1<<1);
	PINSEL0 &= ~(1<<0);
	
	// reset the PWM Timer Counter and PWM Prescale Counter
	// PWMTCR[1] -- Counter Reset
	PWMTCR |= (1<<1);

	
	// Prescalre value for the counter
	// PWMPR - divide the PCLK 
	// since PCLK is 60MHz, let's divide by 15, to get 4MHz - 0.25us
	// divide by 15 - count from 0 to 14
	PWMPR = 14;
	
	/* for sinlge edge and channel 1, we use set by Match Reg0 and reset by Match Reg1 */
	// so we set the needed values
	// PWMMR0 > PWMMR1 for correct operation
	// since, we are having input time of 0.25, so to get us, we multiply by 4
	PWMMR0 = (4*totalTimeus);
	
	// since, we are having input time of 0.25, so to get us, we multiply by 4
	PWMMR1 = (4*onTimeus);
	
	/* Now, we give what should happen when PWMTC reaches PWMMR0 and PWMMR1 */
	// when PWMTC reaches PWMMR0, PWMTC should reset back to 0 for further PWM
	// we do this by setting PWMMR0R - PWMMCR[1]
	PWMMCR |= (1<<1);
	
	// when PWMTC reaches PWMMR1, nothing should be done on PWMTC so we leave it now
	
	/* Selecting the edge and enabling the output */
	// Since, Channel ONE is always single edge we don't need to choose edge
	
	// Enabling the channel ONE is done by
	// PWMENA1 - PWMPCR[9]
	PWMPCR |= (1<<9);
	
	// enabling the PWM Mode
	// PWMTCR[3] -- PWM Enable
	PWMTCR |= (1<<3);
	
	// removing the Counter Reset for operation
	// PWMTCR[1] -- Counter Reset
	PWMTCR &= ~(1<<1);
	
	// enabling the counter to star the counter
	// PWMTCR[0] -- Counter Enable
	PWMTCR |= (1<<0);
}

void PWM_double_edge_channel_TWO()
{
	// select the functionality for PWM2 - P0.7 == 10
	PINSEL0 |= (1<<15);
	PINSEL0 &= ~(1<<14);
		
	// reset the PWM Timer Counter and PWM Prescale Counter
	// PWMTCR[1] -- Counter Reset
	PWMTCR |= (1<<1);

	
	// Prescalre value for the counter
	// PWMPR - divide the PCLK 
	// here we use 500
	PWMPR = 500;
	
	/* for double edge and channel 2, we use set by Match Reg1 and reset by Match Reg 2 */
	// so we set the needed values
	// PWMMR1 > PWMMR2 for correct operation
	PWMMR1 = 1000;
	
	PWMMR2 = 254;
	

	/* Now, we give what should happen when PWMTC reaches PWMMR1 and PWMMR2 */
	/* nothing should be done, as setting and reseting is done automatically */
	
	// when PWMTC reaches PWMMR1, PWMTC should reset back to 0 for further PWM
	// we do this by setting PWMMR1R - PWMMCR[4]
	PWMMCR |= (1<<4);
	
	
	// when PWMTC reaches PWMMR2, nothing should be done on PWMTC so we leave it now
	
	
	/* Selecting the edge and enabling the output */
	// selecting double edge for channel 2 by setting PWMSEL2 - PWMPCR[2]
	PWMPCR |= (1<<2);
	
	
	// Enabling the channel TWO is done by
	// PWMENA2 - PWMPCR[10]
	PWMPCR |= (1<<10);
	
	
	// enabling the PWM Mode
	// PWMTCR[3] -- PWM Enable
	PWMTCR |= (1<<3);
	
	// removing the Counter Reset for operation
	// PWMTCR[1] -- Counter Reset
	PWMTCR &= ~(1<<1);
	
	// enabling the counter to star the counter
	// PWMTCR[0] -- Counter Enable
	PWMTCR |= (1<<0);
	
}

int main()
{
	PLL_clock_change();
	APB_Divder();
	
	// output can be seen in P0.0
	// PWM_single_edge_channel_ONE();
	
	// output can be seen in P0.1 and P0.7
	// PWM_single_edge_channel_TWO_THREE();
	
	// output can be seen in P0.0
	// PWM_single_edge_generatePWM_channel_ONE(125,275);
	
	// output can be seen in P0.1
	// PWM_double_edge_channel_TWO();
	while(1)
	{	

	}
	
}

