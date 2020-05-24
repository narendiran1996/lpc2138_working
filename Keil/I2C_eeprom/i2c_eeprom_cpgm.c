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


// This address is 7bit without any shifting for R/W bit as my implmentation of I2C takes care of it
#define SLAVE_ADDRESS 0b1010000

void EEPROM_writeByte(unsigned char data_byte, unsigned char block_addres, unsigned char word_address)
{
	i2c_set_pos_ack();
	// start condition
	i2c_start_condition();
	
	// Enter Master Transmiter Mode -0
	i2c_enter_MasterMode(SLAVE_ADDRESS | block_addres ,0);
	
	// Accoding to datasheet of EEPROM, the word address iss ent
	i2c_dataTransmit(word_address);
	
	// Next the actual data is sent
	i2c_dataTransmit(data_byte);
	
	// Leave Master Tranmitter Mode -0 and stop condition
	i2c_stop_condition();
}
void EEPROM_writeString(unsigned char *my_str, unsigned char block_address, unsigned char word_address)
{
	unsigned char string_count=0;
	while(*my_str != '\0')
	{
		EEPROM_writeByte(*my_str++, block_address , word_address + string_count);
		custom_delay(1);
		string_count++;
	}
	
}
unsigned char EEPROM_readByte(unsigned char block_address, unsigned char word_address)
{
	i2c_set_pos_ack();
	
	// start condition
	i2c_start_condition();
	
	// For reading first word address needs to be sent  so we enter master transmitter
	// Enter Master Transmiter Mode -0
	i2c_enter_MasterMode(SLAVE_ADDRESS | block_address,0);
	
	i2c_dataTransmit(word_address);
	
	i2c_stop_condition();
	
	i2c_set_neg_ack();
	//Next, we can read
	i2c_start_condition();
	// Enter Master Receiver Mode -0
	i2c_enter_MasterMode(SLAVE_ADDRESS | block_address,1);
	
	unsigned char value_ = i2c_dataReceive();	
	
	i2c_stop_condition();
	
	return value_;
}
void EEPROM_read_string(unsigned char *rect_str,unsigned char lenght_,unsigned char block_address, unsigned char word_address)
{
	
	
	unsigned char i;
	for (i=0;i<lenght_;i++)
	{
		 rect_str[i] = EEPROM_readByte(block_address,word_address + i);	
		custom_delay(1);
	}
	rect_str[i] = '\0';
}



int main()
{
	PLL_clock_change();
	APB_Divder();


	lcd_init();
	lcd_send_string("VAL: ");
	
	i2c_master_init();
	
	EEPROM_writeByte('M', 0, 0);
	custom_delay(1);
	lcd_set_cursor(1,7);
	unsigned char zs=EEPROM_readByte(0,0);
	lcd_send_data(zs);
	
	custom_delay(1);
	EEPROM_writeString("abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ",0,0);
	custom_delay(1);
	lcd_set_cursor(2,1);
	
	unsigned char *kkjl;
	EEPROM_read_string(kkjl,16,0,5);
	lcd_send_string(kkjl);
	
	while(1)
	{
	}
}
