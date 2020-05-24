#include<LPC213x.h>
#include "naren_lcd_include_all_func.h"

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

#define ss_high IO0SET |= (1<<7);
#define ss_low IO0CLR |= (1<<7);

void init_spi()
{
	/* MAKING SPI_SCK, SPI_MISO, SPI_MOSI, SPI_SSEL as corresponding function */
	// MAKING SPI_SCK in P0.4 -- PINSEL0[9:8] = 01
	PINSEL0 |= (1<<8);
	PINSEL0 &= ~(1<<9);
	
	// MAKING SPI_MISO in P0.5 -- PINSEL0[11:10] = 01
	PINSEL0 |= (1<<10);
	PINSEL0 &= ~(1<<11);
	
	// MAKING SPI_MOSI in P0.6 -- PINSEL0[13:12] = 01
	PINSEL0 |= (1<<12);
	PINSEL0 &= ~(1<<13);
	
	// making SPI_SSEL as output
	IO0DIR |= (1<<7);
	
	// disbale the chip
	ss_high;
	

	
	/* *******CONFIGURATION********* */	
	// use 8 bits per transfer -- S0SPCR[2] -> BitEnable = 0
	S0SPCR &= ~(1<<2);
	
	// Sampled at Rising or Falling Edge
	// we choose rising edge  -- S0SPCR[3] -> CPHA = 0
	S0SPCR &= ~(1<<3);
	
	// Let the clock polarity be SCK is low when idle -- S0SPCR[4] -> CPOL = 0
	S0SPCR &= ~(1<<4);
	
	// Select this as Master -- S0SPCR[5] -> MSTER = 1
	S0SPCR |= (1<<5); 
	
	// Send MSB First -- S0SPCR[6] -> LSBF = 0
	S0SPCR &= ~(1<<6);
	
	// Interrrupt disbled -- S0SPCR[7] -> SPIE = 0
	S0SPCR &= ~(1<<7);
	
		/* Selecting clk */
	// Lets select SCK to be 400khz
	// SCK = P_CLK / S0SPCCR
	// 400kHz = 20MHz / S0SPCCR -> S0SPCCR  = 50
	S0SPCCR = 50;
}
unsigned char SPI_transfer_byte(unsigned char data_)
{
	S0SPDR = data_;
	
	// wait till serial transmission is complete by checking the SPI Interrupt Flag
	while((S0SPSR & (1<<7)) == 0 ) {};

	// return the recieved data - can use it or ignore it

	return S0SPDR;
}

void EEPROM_write_string(int address_,unsigned char *c_data)
{
	ss_low
	SPI_transfer_byte(0X06); // WREN instruction -- according to EEPROM
	ss_high
	custom_delay(0);

	
	ss_low
	SPI_transfer_byte(0X02); // WRITE instruction -- according to EEPROM
	// since  address is 16 bi
	SPI_transfer_byte(address_>>8); // sendiNG MSB data first
	SPI_transfer_byte(address_); // address where data should be writen
	
	while(*c_data != '\0')
	{
		SPI_transfer_byte(*c_data++); // data to be written
	}
	
	ss_high
}
void EEPROM_write_byte(int address_, unsigned char data_)
{	
	ss_low
	SPI_transfer_byte(0X06); // WREN instruction -- according to EEPROM
	ss_high
	
	custom_delay(0);
	
	ss_low
	SPI_transfer_byte(0X02); // WRITE instruction -- according to EEPROM
	
	// since  address is 16 bi
	SPI_transfer_byte(address_>>8); // sendiNG MSB data first
	SPI_transfer_byte(address_); // address where data should be writen	
	
	SPI_transfer_byte(data_); // data to be written
	
	ss_high
	
	
	custom_delay(0);
}

unsigned char EEPROM_read(int address_)
{
	unsigned char data_;
	ss_low
	SPI_transfer_byte(0x03); // READ instruction -- according to EEPROM
	
	
	SPI_transfer_byte(address_>>8); // sendiNG MSB data first
	SPI_transfer_byte(address_); // address where data should be writen
	
	data_=SPI_transfer_byte(0XFF); // send empty value to get data from it
	
	ss_high
	
	custom_delay(0);
	return data_;
}

int main()
{
	PLL_clock_change();
	APB_Divder();
	
	lcd_init();
	lcd_send_string("GOOD EVENING");
	
	init_spi();
	EEPROM_write_string(0x0000,"i know");
	custom_delay(1);
	for(unsigned char i=0; i<16; i++)
	{
		unsigned char val = EEPROM_read(0x00+i);
		lcd_set_cursor(2,1+i);
		lcd_send_data(val);
	}
	
	
	while(1)
	{
	
	}
}
