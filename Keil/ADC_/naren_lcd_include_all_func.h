#ifndef __naren_lcd_include_all_func_H
#define __naren_lcd_include_all_func_H

// connect P1.26 TO RS
// connect P1.27 TO EN
// connect P1.28:P1.31 TO D4:D7
#define set_En IO1SET |= (1<<27);
#define clear_En IO1CLR |= (1<<27);


#define set_RS IO1SET |= (1<<26);
#define clear_RS IO1CLR |= (1<<26);

#define set_MSB(val_)	{\
						IO1CLR |= 0XF0000000;\
						IO1SET |= ((val_ & 0x000000F0)<<24);\
						}

#define set_LSB(val_)	{\
						IO1CLR |= 0XF0000000;\
						IO1SET |= ((val_ & 0x0000000F)<<28);\
						}

void custom_delay(unsigned char xyz);
void port_init_lcd(void);
void lcd_send_cmd(unsigned int cmd_);						
void lcd_send_data(unsigned int data_);
void lcd_clear(void);
void lcd_home(void);
void initialization_lcd(void);
void lcd_print_left(void);
void lcd_print_right(void);
void lcd_shift_left(void);
void lcd_shift_right(void);
void lcd_display_off(void);
void lcd_display_on_cursor_off_blink_off(void);
void lcd_display_on_cursor_on_blink_off(void);
void lcd_display_on_cursor_off_blink_on(void);
void lcd_display_on_cursor_on_blink_on(void);
void lcd_cursor_leftShift(void);
void lcd_cursor_rightShift(void);
void lcd_display_leftShift(void);
void lcd_display_rightShift(void);
void lcd_send_string(char *c_string);
void lcd_set_cursor(unsigned char  r,unsigned char  c);
void send_string_l(unsigned char  *c_string,unsigned char  len);
void lcd_init(void);
void lcd_print_num(int num, int base);
						
#endif
