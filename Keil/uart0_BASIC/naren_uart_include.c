#include "naren_uart_include.h"
#include<LPC213x.h>

void uart0_init()
{
	// WE SELECT TXD0 in P0.0 - [1:0]=01
	PINSEL0 = PINSEL0 & ~(1<<1);
	PINSEL0 = PINSEL0 | (1<<0);
	// WE SELECT RXD0 in P0.1 - [3:2]=01
	PINSEL0 = PINSEL0 & ~(1<<3);
	PINSEL0 = PINSEL0 | (1<<2);
	
	/* we know that P_CLK is 20MHz,
	   so accordingly to table
	   U0DLM = 0X00, U0DLL = 0X82, DIVADDVAL = 0, MULVAL = 1
			*/
	// we have to set the DLAB bit in U0LCR to set this values
	U0LCR = U0LCR | (1<<7);
	U0DLM = 0X00;
	U0DLL = 0X82;
	U0FDR = 0b00010000;
	
	// setting 8 bit lenght, 1 stop bit, no parity, disbale break control
	// disbale DLAB bit(Divisor Latch access bit)
	
	// setting 8 bit - [1:0]==11
	U0LCR  = U0LCR | (1<<1) | (1<<0);
	
	// setting 1 stop bit - [2] = 0
	U0LCR = U0LCR & ~(1<<2);
	
	// setting parrity disbale - [3] = 0
	U0LCR = U0LCR & ~(1<<3);
	
	// setting disable break control - [6] = 0
	U0LCR = U0LCR & ~(1<<6);
	
	// setting DLAB bit [7] to 0
	U0LCR = U0LCR & ~(1<<7);
}
void uart0_transmit_char(char val_)
{
	// giving the value to U0THR to send
	U0THR = val_;
	// chaking the Transmitter Empty bit[6] to indicate the end of transmiition
	while( (U0LSR & (1<<6)) == 0 );
}
void uart0_transmit_string(char *vals)
{
	while(*vals!='\0')
	{
		uart0_transmit_char(*vals++);
	}
}
char uart0_receive_char()
{
	// wait for thedate to be recied
	while( (U0LSR & 0x01) == 0);
		
	return U0RBR;
}
char *usart0_receive_string_till(unsigned char deliminator)
{
	int i=0;
	char rec_buff[1024];
	char curr_char = uart0_receive_char();
	while(curr_char != deliminator)
	{
		rec_buff[i] = curr_char;
		curr_char = uart0_receive_char();
		i++;
	}
	rec_buff[i] = '\0';
	return rec_buff;
}
