#ifndef __SPILCD_H__
#define __SPILCD_H__

void lcd_port_setup();
void lcd_init();
void lcd_puts(char *c);
void lcd_goto(int r, int c);
void lcd_clear_and_home();

#endif