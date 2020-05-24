#ifndef __naren_uart1_include_H
#define __naren_uart1_include_H

void uart1_init(void);
void uart1_transmit_char(char val_);
void uart1_transmit_string(char *vals);
char uart1_receive_char(void);
char *uart1_receive_string_till(unsigned char deliminator);

#endif
