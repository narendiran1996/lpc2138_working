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
int init_adc_read_single_ADC0_3()
{
	
	// Selecting the ADC channel input pins
	// by choosing SEL bits in AD0CR[7:0]
	// let's choose AD0.3
	AD0CR = (AD0CR & ~0xFF) | 0b00001000;

	
	// selecting the function for P0.30 as AD0.3
	// PINSEL1[29:28] - 01
	PINSEL1 &= ~(1<<29);
	PINSEL1 |= (1<<28);
	
	// Selecting the clcok divider for ADC
	// by selecting CLKDIV bit in AD0CR[15:8]
	// since PCLK is 60MHz, to get 4.5MHz -- we need 60/4.5 = 13.33
	// we choose 14 to get 4.5MHz or lesss, we can't choose 13, as it would be more than 4.5MHz
	// But, we have to write 13 as CLKDIV+1 is used for division
	AD0CR = (AD0CR & ~0xFF00) | (13 <<8);
	
	// Now, we have to select between continous and single
	// let's select single
	// by clearing BURST bit in AD0CR[16]
	AD0CR &= ~(1<<16);
	
	// we don't need to select CLKS as it is applicable only in BURST mode
	
	// we have to switch on the ADC
	// by setting PDN in AD0CR[21]
	AD0CR |= (1<<21);
	
	
	// now, we can start the adc converstion when we want
	// let's startnow by giving 001 to START in AD0CR[26:24]
	AD0CR &= ~(1<<26);
	AD0CR &= ~(1<<25);
	AD0CR |= (1<<24);
		
		
	// checking if the conversion is over by checking the DONE3 flag in AD0STAT
	while( (AD0GDR & (1<<31)) == 0);
	// or by cheking done flag in corresponding register 
	// DONE in ADDR[31]
	// while( (AD0DR3 & (1<<31)) == 0);
	
	// returning the result from ADDR3[15:6] 
	//while( (AD0DR & (1<<31)) == 0);
	
	//return (AD0DR & 0x1FFC0) >>6;
	return (AD0GDR & 0x1FFC0) >>6;
}
void init_adc_read_continous_ADC0_2()
{
	
	// Selecting the ADC channel input pins
	// by choosing SEL bits in AD0CR[7:0]
	// let's choose AD0.1
	AD0CR = (AD0CR & ~0xFF) | 0b00000100;

	
	// selecting the function for P0.29 as AD0.2
	// PINSEL1[27:26] - 01
	PINSEL1 &= ~(1<<27);
	PINSEL1 |= (1<<26);
	
	// Selecting the clcok divider for ADC
	// by selecting CLKDIV bit in AD0CR[15:8]
	// since PCLK is 60MHz, to get 4.5MHz -- we need 60/4.5 = 13.33
	// we choose 14 to get 4.5MHz or lesss, we can't choose 13, as it would be more than 4.5MHz
	// But, we have to write 13 as CLKDIV+1 is used for division
	AD0CR = (AD0CR & ~0xFF00) | (13 <<8);
	
	// Now, we have to select between continous and single
	// let's select continous
	// by clearing BURST bit in AD0CR[16]
	AD0CR |= (1<<16);
	
	// we need to set the CLKS field -- A0CR[19:17]
	// let'S Seclect 10bit precision	- 000
	AD0CR &= ~(1<<19);
	AD0CR &= ~(1<<18);
	AD0CR &= ~(1<<17);
	
	
	// we have to switch on the ADC
	// by setting PDN in AD0CR[21]
	AD0CR |= (1<<21);
	
}


//int main()
//{
//	PLL_clock_change();
//	APB_Divder();
//	lcd_init();
//	lcd_display_on_cursor_off_blink_off();
//	
//	lcd_send_string("What's your prob");
//	
//	init_adc_read_continous_ADC0_2();
//	
//	
//	while(1)
//	{
//		//		int abc = init_adc_read_single_ADC0_3();
//		//	lcd_set_cursor(2,1);
//		//	lcd_print_num(abc,10);
//		if((AD0GDR & (1<<31)) != 0)
//		{
//			int abc = (AD0GDR & 0x1FFC0) >>6;
//			lcd_set_cursor(2,5);
//			lcd_print_num(abc,10);
//		}
//	}
//}
#include <stdint.h>
#include <stdio.h>
#include <string.h>
int main(void)
{
	PLL_clock_change();
	APB_Divder();
	uint32_t result;
	float voltage;
	char volt[18];
	lcd_init();
	lcd_display_on_cursor_off_blink_off();
	lcd_send_string("your prob");
	PINSEL1 = 0x01000000; /* P0.28 as AD0.1 */
	AD0CR = 0x00200402; /* ADC operational, 10-bits, 11 clocks for conversion */
	while(1)
	{
		AD0CR = AD0CR | (1<<24); /* Start Conversion */
		while ( !(AD0DR1 & 0x80000000) ); /* Wait till DONE */
		result = AD0DR1;
		result = (result>>6);
		result = (result & 0x000003FF);
		voltage = ( (result/1023.0) * 3.3 ); /* Convert ADC value to equivalent voltage */
		lcd_set_cursor(2,5);
		sprintf(volt, "Voltage=%.2f V  ", voltage);
		lcd_send_string(volt);
		memset(volt, 0, 18);
	}
}