#ifndef __SPILCD_H__
#define __SPILCD_H__

void lcd_port_setup();
void lcd_init();
void lcd_putc(char c);
void lcd_goto(int x, int y);

#endif