#ifndef __naren_uart_include_H
#define __naren_uart_include_H

void uart0_init(void);
void uart0_transmit_char(char val_);
void uart0_transmit_string(char *vals);
char uart0_receive_char(void);
char *usart0_receive_string_till(unsigned char deliminator);

#endif
