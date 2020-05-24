#include<LPC213x.h>
#include "naren_uart1_include.h"


void uart1_init()
{
	// WE SELECT TXD1 in P0.8 - [17:16]=01
	PINSEL0 = PINSEL0 & ~(1<<17);
	PINSEL0 = PINSEL0 | (1<<16);
	// WE SELECT RXD1 in P0.9 - [19:18]=01
	PINSEL0 = PINSEL0 & ~(1<<19);
	PINSEL0 = PINSEL0 | (1<<18);
	
	/* we know that P_CLK is 20MHz,
	   so accordingly to table
	   U1DLM = 0X00, U1DLL = 0X82, DIVADDVAL = 0, MULVAL = 1
			*/
	// we have to set the DLAB bit in U1LCR to set this values
	U1LCR = U1LCR | (1<<7);
	U1DLM = 0X00;
	U1DLL = 0X82;
	U1FDR = 0b00010000;
	
	// setting 8 bit lenght, 1 stop bit, no parity, disbale break control
	// disbale DLAB bit(Divisor Latch access bit)
	
	// setting 8 bit - [1:0]==11
	U1LCR  = U1LCR | (1<<1) | (1<<0);
	
	// setting 1 stop bit - [2] = 0
	U1LCR = U1LCR & ~(1<<2);
	
	// setting parrity disbale - [3] = 0
	U1LCR = U1LCR & ~(1<<3);
	
	// setting disable break control - [6] = 0
	U1LCR = U1LCR & ~(1<<6);
	
	// setting DLAB bit [7] to 0
	U1LCR = U1LCR & ~(1<<7);
}
void uart1_transmit_char(char val_)
{
	// giving the value to U1THR to send
	U1THR = val_;
	// chaking the Transmitter Empty bit[6] to indicate the end of transmiition
	while( (U1LSR & (1<<6)) == 0 );
}
void uart1_transmit_string(char *vals)
{
	while(*vals!='\0')
	{
		uart1_transmit_char(*vals++);
	}
}
char uart1_receive_char()
{
	// wait for thedate to be recied
	while( (U1LSR & 0x01) == 0);
		
	return U1RBR;
}
char *uart0_receive_string_till(unsigned char deliminator)
{
	int i=0;
	char rec_buff[1024];
	char curr_char = uart1_receive_char();
	while(curr_char != deliminator)
	{
		rec_buff[i] = curr_char;
		curr_char = uart1_receive_char();
		i++;
	}
	rec_buff[i] = '\0';
	return rec_buff;
}
