#include "spilcd.h"

#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"

static uint8_t backlight_config = 0x00;

inline static void delay_us(uint32_t t) { SysCtlDelay(26 * t); }

inline static void delay_ms(uint32_t t) { delay_us(1000 * t); }

static void lcd_shift(uint8_t d) {
  SSIDataPut(SSI2_BASE, d);
  while (SSIBusy(SSI2_BASE))
    ;
}

static void lcd_shift_nibble(uint8_t d) {
  lcd_shift(backlight_config | d);
  lcd_shift(backlight_config | d | 0x10);
  lcd_shift(backlight_config | d);
}

static void lcd_write_cmd(uint8_t d) {
  lcd_shift_nibble((d & 0xf0) >> 4);
  delay_ms(5);
  lcd_shift_nibble(d & 0x0f);
  delay_ms(5);
}

static void lcd_write_data(uint8_t d) {
  lcd_shift_nibble(0x20 | ((d & 0xf0) >> 4));
  delay_us(200);
  lcd_shift_nibble(0x20 | (d & 0x0f));
  delay_us(200);
}

void lcd_port_setup() {
  SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  GPIOPinConfigure(GPIO_PB4_SSI2CLK);
  GPIOPinConfigure(GPIO_PB5_SSI2FSS);
  GPIOPinConfigure(GPIO_PB6_SSI2RX);
  GPIOPinConfigure(GPIO_PB7_SSI2TX);
  GPIOPinTypeSSI(GPIO_PORTB_BASE,
                 GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7);
  SSIConfigSetExpClk(SSI2_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
                     SSI_MODE_MASTER, 500000, 8);
  SSIEnable(SSI2_BASE);
}

void lcd_init() {
  delay_ms(20);
  lcd_write_cmd(0x33);
  lcd_write_cmd(0x32);
  lcd_write_cmd(0x28);
  lcd_write_cmd(0x08);
  lcd_write_cmd(0x01);
  lcd_write_cmd(0x06);
  lcd_write_cmd(0x0c);
}

void lcd_puts(char *c) {
  for (; *c; c++) {
    lcd_write_data(*c);
  }
}

void lcd_goto(int r, int c) {
  switch (r) {
  case 0:
    lcd_write_cmd(0x80 + c);
    break;
  case 1:
    lcd_write_cmd(0xC0 + c);
    break;
  case 2:
    lcd_write_cmd(0x94 + c);
    break;
  case 3:
    lcd_write_cmd(0xD4 + c);
    break;
  }
}

void lcd_clear_and_home() {
  lcd_write_cmd(0x01);
}
