#include "naren_i2c_master_include.h"
#include <LPC213X.h>
unsigned char status;
unsigned char i2c_get_status()
{
	// Status value are available from I2CSTAT)[7:3]
	return I2C0STAT & 0XF8;
}
void i2c_master_init()
{
	// making SCL0 function for P0.2 - 01
	PINSEL0 |= (1<<4);
	PINSEL0 &= ~(1<<5);
	
	// making SDA0 function for P0.3 - 01
	PINSEL0 |= (1<<6);
	PINSEL0 &= ~(1<<7);
	
	// first I2C must be enabled as MASTTER by writing into I2C0CONSET registe
	// I2C0CONSET[6] - I2EN = 1
	I2C0CONSET |= (1<<6);
	
	// I2C0CONSET[5] - STA = 0 -- clearing using I2C0CONCLR
	I2C0CONCLR |= (1<<5);
	
	// I2C0CONSET[4] - STO = 0 -- clearing using I2C0CONCLR
	I2C0CONCLR |= (1<<4);

	// I2C0CONSET[3] - SI = 0 -- clearing using I2C0CONCLR
	I2C0CONCLR |= (1<<3);
	
	// Next, the I2C frequeny is set to 400khz
	// since PCLK is 20MHz -> I2SCCL+I2SCCH = 50 (both should not be equal)
	I2C0SCLL = 35;
	I2C0SCLH = 15;

}

unsigned char i2c_start_condition()
{
	// I2C0CONSET[4] - STO = 0 -- clearing using I2C0CONCLR
	I2C0CONCLR |= (1<<4);
	
	/* we enter the master transmiiter by settin STA bit */
	// I2C0CONSET[5] - STA = 1
	I2C0CONSET	|= (1<<5);
	
	
	// Checking if START condition is sent correctly
	// by checking I2C0CONSET[3] -  SI bit
	while((I2C0CONSET & (1<<3 )) == 0x00);
	status = i2c_get_status();
	
	// checking status if START condition is sent correctily
	if(status != 0x08)
	{
		// error occured
		return 1;
	}
	return 0;	
}

unsigned char i2c_stop_condition()
{
	/* we enter stop condition by STO bit */
	// I2C0CONSET[4] - STO = 1
	I2C0CONSET	|= (1<<4);
	

	return 0;
}


unsigned char i2c_enter_MasterMode(unsigned char slave_address, unsigned char transmiter0_receive1)
{
	// Writing SLA+W into I2C0DAT for transmiiter and SLA+R for receiver
	// slave address must be MSB first
	// slave address is left shifted by 1 in order to accompany the R/W bit
	I2C0DAT = (slave_address<<1) | transmiter0_receive1;
	
	// Do the transaction by clearing the SI bit in I2C0CONCLR
	// I2C0CONSET[3] -  SI bit
	I2C0CONCLR	|= (1<<3);
	
	while((I2C0CONSET & (1<<3 )) == 0x00);
	status = i2c_get_status();
	
	
	// For transmitter the staus would have to be 0x18 and for receiver 0x40
	unsigned char status_val_checker = (transmiter0_receive1==0) ? 0x18 : 0x40;
	if(status != status_val_checker)
	{
		// error occured
		return 1;
	}
	return 0;
}
unsigned char i2c_dataTransmit(unsigned char data_)
{
	// Data packet is transmitted
	// Writing data intor I2C0DAT
	I2C0DAT = data_;
	
	// Do the transaction by clearing the SI bit in I2C0CONCLR
	// I2C0CONSET[3] -  SI bit
	I2C0CONCLR	|= (1<<3);
	
	while((I2C0CONSET & (1<<3 )) == 0x00);
	status = i2c_get_status();
	
	if(status != 0x28)
	{
		// error occured
		return 1;
	}
	return 0;
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

unsigned char i2c_dataReceive()
{
	unsigned char value_;
	
	// Do the transaction by clearing the SI bit in I2C0CONCLR
	// I2C0CONSET[3] -  SI bit
	I2C0CONCLR	|= (1<<3);
	
	// Do the transaction
	while((I2C0CONSET & (1<<3 )) == 0x00)
	{
		value_ = I2C0DAT;
	}

	status = i2c_get_status();
	if(status !=  0x58)
	{
		// error occured
		return 1;
	}

	return value_;
}

