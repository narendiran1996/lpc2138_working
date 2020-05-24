#include<LPC213x.h>
#include "naren_lcd_include_all_func.h"
#include "naren_uart_include.h"

#define X_CPU 10000000L
#define C_CLK 40000000L // since multiplier in PLL0 is 4
#define P_CLK 20000000L // since vpd_div is made 10 - repressing C_CLK/2

void PLL_clock_change()
{
	/* Enabling PLL Feed first */
	// PLLCON[0] = PLLE
	PLLCON  |= (1<<0);
	
	/* the F_OSC is 10MHz, so let's make the C_CLK to 40MHz */
	/* we know that CCLK = M x F_OSC */
	// so M would be 4,
	// to get M of 4, 3 should be written to MSEL
	// PLLCFG[4:0] = MSEL
	PLLCFG = (PLLCFG  & 0b11100000) | 3;
	
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
	// making bit[1:0] as 10 to get 1/2th CCLK
	VPBDIV = VPBDIV | (1<<1);
	VPBDIV = VPBDIV & ~(1<<0);
}


int main()
{
	PLL_clock_change();
	APB_Divder();
	lcd_init();
	
	uart0_init();
	uart0_transmit_string("Will this work\n\r");
	
	while(1)
	{
		char *abc = usart0_receive_string_till('\n');
		//uart0_transmit_char(abc);
		lcd_set_cursor(1,1);
		
		lcd_send_string("rec: ");
		lcd_send_string(abc);
	}
}
