#ifndef __naren_i2c_master_include_H
#define __naren_i2c_master_include_H


unsigned char i2c_get_status(void);
void i2c_master_init(void);
unsigned char i2c_start_condition(void);
unsigned char i2c_stop_condition(void);
unsigned char i2c_enter_MasterMode(unsigned char slave_address, unsigned char transmiter0_receive1);
unsigned char i2c_dataTransmit(unsigned char data_);
unsigned char i2c_dataReceive(void);
void i2c_set_pos_ack(void);
void i2c_set_neg_ack(void);

#endif
