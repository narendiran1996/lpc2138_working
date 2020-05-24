#include "naren_lcd_include_all_func.h"
#include<LPC213x.h>

void custom_delay(unsigned char xyz)
{
	int i,j;
	for(i=0;i<123;i++)
	{
		for(j=0;j<xyz*123;j++){}
	}
}
void port_init_lcd()
{
	IO1DIR |= 0XFC000000;
	IO1CLR |= 0XFC000000;
}

void lcd_send_cmd(unsigned int cmd_)
{
	set_MSB(cmd_)
	clear_RS
	set_En
	custom_delay(0);
	clear_En
	
	set_LSB(cmd_)
	clear_RS
	set_En
	custom_delay(0);
	clear_En
}
void lcd_send_data(unsigned int data_)
{
	set_MSB(data_)
	set_RS
	set_En
	custom_delay(0);
	clear_En
	
	set_LSB(data_)
	set_RS
	set_En
	custom_delay(0);
	clear_En
}

void initialization_lcd()
{
	/* The module powers up in 8-bit mode.
	   The initial start-up instructions are sent in 8-bit mode, 
	   with the lower four bits (which are not connected) of each instruction
	    as don't cares */
	lcd_send_cmd(0x02);	// initialization in 4-bit mode
	
	/* setting 4bit communication and 2-line display and 5x8 dots format display */
	// Function Set --> D[7:4] -- 001(DL)
	// DL === 0 -- 4bit mode
	// DL === 1 -- 8bit mode
	// D[3:0] -- (N)(F)xx
	// N === 0 -- 1-line display
	// N === 1 -- 2-line display
	// F === 0 -- 5x8 dots format display mode
	// F === 1 -- 5x11 dots format display mode
	// so we select D[7:0] = 0b0010_10xx --- 0x28
	lcd_send_cmd(0x28);
	
	/* Setting up of cursor left movement and display not shifting */
	/* Entry Mode */
	// Entry Mode Set --> D[7:0] -- 0000_01(I/D)(S)
	// I/D === 0 -- Cursor/Blink moves to left and DDRAM address is decreased by 1
	// I/D === 1 -- Cursor/Blink moves to righ and DDRAM address is decreased by 1
	// S === 0 -- Dont' Shifht the entire display
	// S === 1 -- Shift the entire display
	// so we select D[7:0] = 0b0000_0110 --- 0x06
	lcd_send_cmd(0x06);
		
	/* Setting up of disply on, cursor on and blink on	*/
	/* Display ON/OFF, Cursor ON/OFF  Blink ON/OFF*/
	// Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
	// D === 0 -- display off display data is remained in DDRAM
	// D === 1 -- display on
	// C === 0 -- cursor is disabled in current display, but I/D register data is reamins
	// C === 1 -- cursto is turned on
	// B === 0 -- Blink off
	// B === 1 -- Blink on	
	// so we slect D[7:0] = 0b0000_1111 = 0x0F;
	lcd_send_cmd(0x0E);
	

	/* Set DDRAM address */
	// when 1- line display (N=0), DDRAM address from 00H to 4FH -- TOTAL 40
	// when 2- line display (N=1), DDRAM address from 00H to 27H for first line AND 40H to 67H for seoncd ine -- totall 20 + 20 = 40
	// D[7:0] --- 0b1(AC6)(AC5)(AC4)(AC3)(AC2)(AC1)(AC0)	
	// So we select D[7:0] = 0b1000_0000 = 0x80;
	
	lcd_send_cmd(0x80);
}


void lcd_clear()
{	
	// 0x01 for clearing display
	lcd_send_cmd(0x01);	
}
void lcd_home()
{
	// 0000_001x for moving cursor to inital position and moving DRAM address to 00H
	lcd_send_cmd(0x02);
}
void lcd_print_left()
{
	// moving left
	/* Entry Mode */
	// Entry Mode Set --> D[7:0] -- 0000_01(I/D)(S)
	// I/D === 0 -- Cursor/Blink moves to left and DDRAM address is decreased by 1
	// I/D === 1 -- Cursor/Blink moves to right and DDRAM address is decreased by 1
	// S === 0 -- Dont' Shifht the entire display
	// S === 1 -- Shift the entire display
	// so we select D[7:0] = 0b0000_0100 --- 0x04
	lcd_send_cmd(0x04);
}
void lcd_print_right()
{
	// moving left
	/* Entry Mode */
	// Entry Mode Set --> D[7:0] -- 0000_01(I/D)(S)
	// I/D === 0 -- Cursor/Blink moves to left and DDRAM address is decreased by 1
	// I/D === 1 -- Cursor/Blink moves to right and DDRAM address is decreased by 1
	// S === 0 -- Dont' Shifht the entire display
	// S === 1 -- Shift the entire display
	// so we select D[7:0] = 0b0000_0100 --- 0x06
	lcd_send_cmd(0x06);
}
void lcd_shift_left()
{
	/* Entry Mode */
	// Entry Mode Set --> D[7:0] -- 0000_01(I/D)(S)
	// I/D === 0 -- Cursor/Blink moves to left and DDRAM address is decreased by 1
	// I/D === 1 -- Cursor/Blink moves to right and DDRAM address is decreased by 1
	// S === 0 -- Dont' Shifht the entire display
	// S === 1 -- Shift the entire display
	// so we select D[7:0] = 0b0000_0100 --- 0x06
	lcd_send_cmd(0x07);
}
void lcd_shift_right()
{
	/* Works when there is character before the first character */
	/* Entry Mode */
	// Entry Mode Set --> D[7:0] -- 0000_01(I/D)(S)
	// I/D === 0 -- Cursor/Blink moves to left and DDRAM address is decreased by 1
	// I/D === 1 -- Cursor/Blink moves to right and DDRAM address is decreased by 1
	// S === 0 -- Dont' Shifht the entire display
	// S === 1 -- Shift the entire display
	// so we select D[7:0] = 0b0000_0100 --- 0x06
	lcd_send_cmd(0x05);
}
void lcd_display_off()
{
	// Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
	// D === 0 -- display off display data is remained in DDRAM
	// D === 1 -- display on
	// C === 0 -- cursor is disabled in current display, but I/D register data is reamins
	// C === 1 -- cursto is turned on
	// B === 0 -- Blink off
	// B === 1 -- Blink on
	// so we slect D[7:0] = 0b0000_10xx = 0x0F;
	lcd_send_cmd(0x08);
}
void lcd_display_on_cursor_off_blink_off()
{
	// Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
	// D === 0 -- display off display data is remained in DDRAM
	// D === 1 -- display on
	// C === 0 -- cursor is disabled in current display, but I/D register data is reamins
	// C === 1 -- cursto is turned on
	// B === 0 -- Blink off
	// B === 1 -- Blink on
	// so we slect D[7:0] = 0b0000_1100 = 0x0E;
	lcd_send_cmd(0x0C);
}
void lcd_display_on_cursor_on_blink_off()
{
	// Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
	// D === 0 -- display off display data is remained in DDRAM
	// D === 1 -- display on
	// C === 0 -- cursor is disabled in current display, but I/D register data is reamins
	// C === 1 -- cursto is turned on
	// B === 0 -- Blink off
	// B === 1 -- Blink on
	// so we slect D[7:0] = 0b0000_1110 = 0x0E;
	lcd_send_cmd(0xE);
}
void lcd_display_on_cursor_off_blink_on()
{
	// Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
	// D === 0 -- display off display data is remained in DDRAM
	// D === 1 -- display on
	// C === 0 -- cursor is disabled in current display, but I/D register data is reamins
	// C === 1 -- cursto is turned on
	// B === 0 -- Blink off
	// B === 1 -- Blink on
	// so we slect D[7:0] = 0b0000_1101 = 0x0D;
	lcd_send_cmd(0x0D);
}
void lcd_display_on_cursor_on_blink_on()
{
	// Display On/off --> D[7:0] -- 0000_1(D)(C)(B)
	// D === 0 -- display off display data is remained in DDRAM
	// D === 1 -- display on
	// C === 0 -- cursor is disabled in current display, but I/D register data is reamins
	// C === 1 -- cursto is turned on
	// B === 0 -- Blink off
	// B === 1 -- Blink on
	// so we slect D[7:0] = 0b0000_1111 = 0x0F;
	lcd_send_cmd(0x0F);
}
void lcd_cursor_leftShift()
{
	// Cursor or Display Shift --> D[7:0] -- 0001_(S/C)(R/L)xx
	// (S/C)(R/L) === 00 -- Shift Cursor to left
	// (S/C)(R/L) === 01 -- 
	// (S/C)(R/L) === 10 -- 
	// (S/C)(R/L) === 11 -- 
	// so we slect D[7:0] = 0b0001_00xx = 0x10;
	lcd_send_cmd(0x010);	
}
void lcd_cursor_rightShift()
{
	// Cursor or Display Shift --> D[7:0] -- 0001_(S/C)(R/L)xx
	// (S/C)(R/L) === 00 -- Shift Cursor to left
	// (S/C)(R/L) === 01 -- Shift Cursor to Right
	// (S/C)(R/L) === 10 --
	// (S/C)(R/L) === 11 --
	// so we slect D[7:0] = 0b0001_01xx = 0x14;
	lcd_send_cmd(0x014);	
}
void lcd_display_leftShift()
{
	// Cursor or Display Shift --> D[7:0] -- 0001_(S/C)(R/L)xx
	// (S/C)(R/L) === 00 -- Shift Cursor to left
	// (S/C)(R/L) === 01 -- Shift Cursor to Right
	// (S/C)(R/L) === 10 -- Shift Display to Left
	// (S/C)(R/L) === 11 --
	// so we slect D[7:0] = 0b0001_10xx = 0x18;
	lcd_send_cmd(0x018);
}
void lcd_display_rightShift()
{
	// Cursor or Display Shift --> D[7:0] -- 0001_(S/C)(R/L)xx
	// (S/C)(R/L) === 00 -- Shift Cursor to left
	// (S/C)(R/L) === 01 -- Shift Cursor to Right
	// (S/C)(R/L) === 10 -- Shift Display to Left
	// (S/C)(R/L) === 11 -- Shift Display to Left
	// so we slect D[7:0] = 0b0001_11xx = 0x1C;
	lcd_send_cmd(0x01C);
}
void lcd_send_string(char *c_string)
{
	while(*c_string != '\0')
	{
		lcd_send_data(*c_string++);
	}
}
void lcd_set_cursor(unsigned char  r,unsigned char  c)
{
	// r and c starts from 1
	if (r==1)
	{
		lcd_send_cmd(0x80+c-1);
	}
	else if (r==2)
	{
		lcd_send_cmd(0xC0+c-1);
	}
}

void send_string_l(unsigned char  *c_string,unsigned char  len)
{
	for (unsigned char  i=0;i<len;i++)
	{
		lcd_send_data(c_string[i]);
	}
}
void lcd_init()
{
	port_init_lcd();
	initialization_lcd();
	lcd_display_on_cursor_on_blink_on();
}

char* itoa(int value, char* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

void lcd_print_num(int num, int base)
{
	char buff[16];
	itoa(num,buff, base);
	lcd_send_string(buff);
}
