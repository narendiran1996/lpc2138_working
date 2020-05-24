#include <LPC213X.h>
#include "naren_lcd_include_all_func.h"

#include "naren_i2c_master_include.h"

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

#define SLAVE_ADDRESS 0b01010101
void init_master()
{
	i2c_master_init();
	

	i2c_set_pos_ack();
	i2c_start_condition();
	i2c_enter_MasterMode(SLAVE_ADDRESS,1);
	i2c_set_neg_ack();
	unsigned char rec = i2c_dataReceive();
	i2c_stop_condition();
	
	
	lcd_set_cursor(2,1);
	lcd_send_data(rec);

	i2c_set_pos_ack();
	
	i2c_start_condition();
	
	i2c_enter_MasterMode(SLAVE_ADDRESS,0);
	i2c_dataTransmit('I');
	
	i2c_stop_condition();
	
}


int main()
{
	PLL_clock_change();
	APB_Divder();
	
	lcd_init();
	lcd_send_string("I AM MASTER");
	
	init_master();
	
	
	while(1)
	{
	}
}
