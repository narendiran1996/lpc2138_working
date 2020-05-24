#ifndef __naren_i2c_slave_include_H
#define __naren_i2c_slave_include_H

unsigned char i2c_get_status(void);
unsigned char i2c_slave_init(unsigned char my_address, unsigned char transmitter0_receiver);
unsigned char i2c_dataTransmit(unsigned char data_);
unsigned char i2c_slave_dataReceive(void);
void i2c_set_pos_ack(void);
void i2c_set_neg_ack(void);


#endif
