#include<LPC213x.h>
#include "naren_lcd_include_all_func.h"

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

void init_RTC_EXTERNAL()
{
	// use the external oscillator for RTC
	// by clearing CLKSRC - CCR[4]
	CCR &= ~(1<<4);
	
	// initlaly reseting the Clock Tick Counter
	// by setting CTCRST - CCR[1]
	CCR |= (1<<1);
	
	// dISBALE alarm by masking all bits of Alarm Mask Register 
	AMR = 0xFF;
	
	/* not let's give a particular date time */
	// 30 - JUNE - 1996 - 10:00:00
	
	SEC = 0;
	MIN = 0;
	HOUR = 9;
	
	DOM = 30;
	MONTH = 6;
	YEAR = 1996;
	
	// sunday - 182nd day
	DOW = 0;
	DOY = 182;
	
	
	
	
	// removing the reset of clock tick counter
	// by clearing CTCRST - CCR[1]
	CCR &= ~(1<<1);
	
	// enabling the counter for counter and operation of RTC
	// by setting the CLKEN - CCR[0]
	CCR |= (1<<0);
}
void init_RTC_INTERNAL()
{
	// use the PCLK as SOURCE CLK
	// by setting CLKSRC - CCR[4]
	CCR |= (1<<4);
	
	// initlaly reseting the Clock Tick Counter
	// by setting CTCRST - CCR[1]
	CCR |= (1<<1);
	
	// dISBALE alarm by masking all bits of Alarm Mask Register 
	AMR = 0xFF;
	
	/* not let's give a particular date time */
	// 30 - JUNE - 1996 - 7:00:00
	
	SEC = 00;
	MIN = 0;
	HOUR = 7;
	
	DOM = 30;
	MONTH = 6;
	YEAR = 1996;
	
	// sunday - 182nd day
	DOW = 0;
	DOY = 182;
	
	
	/* setting the prescalre values */
	PREINT = 1830;
	PREFRAC = 1792;
	
	
	// removing the reset of clock tick counter
	// by clearing CTCRST - CCR[1]
	CCR &= ~(1<<1);
	
	// enabling the counter for counter and operation of RTC
	// by setting the CLKEN - CCR[0]
	CCR |= (1<<0);
}

unsigned char get_secs()
{
	return (CTIME0 & 0x3F);
}
unsigned char get_mins()
{
	return ((CTIME0 & 0x3F00) >> 8);
}
unsigned char get_hours()
{
	return ((CTIME0 & 0x1F0000) >> 16);
}


unsigned char get_day()
{
	return (CTIME1 & 0x1F);
}
unsigned char get_month()
{
	return ((CTIME1 & 0xF00) >> 8);
}
int get_year()
{
	return ((CTIME1 & 0xFFF0000) >> 16);
}
void display_lcd()
{
	lcd_set_cursor(1,1);
	lcd_print_num(get_day(),10);
	lcd_send_data('-');
	lcd_print_num(get_month(),10);
	lcd_send_data('-');
	lcd_print_num(get_year(),10);
	
	lcd_set_cursor(2,1);
	lcd_print_num(get_hours(),10);
	lcd_send_data(':');
	lcd_print_num(get_mins(),10);
	lcd_send_data(':');
	lcd_print_num(get_secs(),10);
}

int main()
{
	PLL_clock_change();
	APB_Divder();
	lcd_init();
	lcd_display_on_cursor_off_blink_off();
	
	
	
	init_RTC_INTERNAL();
	while(1)
	{
		display_lcd();
	}
}
