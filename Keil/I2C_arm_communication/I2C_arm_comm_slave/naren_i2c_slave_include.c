#include <LPC213X.h>
#include "naren_i2c_slave_include.h"

unsigned char status;
unsigned char i2c_get_status()
{
	// Status value are available from I2CSTAT)[7:3]
	return I2C0STAT & 0XF8;
}
unsigned char i2c_slave_init(unsigned char my_address, unsigned char transmitter0_receiver)
{
	// making SCL0 function for P0.2 - 01
	PINSEL0 |= (1<<4);
	PINSEL0 &= ~(1<<5);
	
	// making SDA0 function for P0.3 - 01
	PINSEL0 |= (1<<6);
	PINSEL0 &= ~(1<<7);
	
	I2C0ADR = (my_address<<1) & 0xFE; // slave address  and last LSB 0 is for general call
	// Next, I2CONSET is set as follows

	
	// first I2C must be enabled as SLAVE by writing into I2C0CONSET registe
	// I2C0CONSET[6] - I2EN = 1
	I2C0CONSET |= (1<<6);
	
	// I2C0CONSET[5] - STA = 0 -- clearing using I2C0CONCLR
	I2C0CONCLR |= (1<<5);
	
	// I2C0CONSET[4] - STO = 0 -- clearing using I2C0CONCLR
	I2C0CONCLR |= (1<<4);

	// I2C0CONSET[3] - SI = 0 -- clearing using I2C0CONCLR
	I2C0CONCLR |= (1<<3);
	
	// I2C0CONSET[2] - AA = 1
	I2C0CONSET |= (1<<2);
	
	
	// check the SI bit for if its' addressed
	while((I2C0CONSET & (1<<3)) == 0X00){};
	status = i2c_get_status();
	
	if (status != 0x60)
	{
		////error
	    return 0;
	}
	return 1;
	
}
unsigned char i2c_dataTransmit(unsigned char data_)
{
	// Data packet is transmitted
	// Writing data intor TWDR
	I2C0DAT = data_;
	// Do the transaction
	I2C0CONCLR |= (1<<3);
	// Do the transaction
	while((I2C0CONSET & (1<<3)) == 0X00){};
	status = i2c_get_status();
	return status;
}

void i2c_set_pos_ack()
{
	// if recvied successfuly, send ack
	// I2C0CONSET[2] -  AA bit
	I2C0CONSET	|= (1<<2);
}
void i2c_set_neg_ack()
{
	// if recvied the last bye
	// I2C0CONCLR[2] -  AA bit
	I2C0CONCLR	|= (1<<2);
}

unsigned char i2c_slave_dataReceive()
{
	unsigned char value_;
	// Data packet is recieved
	I2C0CONCLR |= (1<<3);
	// Do the transaction
	while((I2C0CONSET & (1<<3)) == 0X00)
	{
		value_ = I2C0DAT;
	}
	status = i2c_get_status();
	if (status == 0x88)
	{
		// received correctly
		return value_;	
	}
	else
	{
		// some error
		return 0xFF;
	}
}
