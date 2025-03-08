#ifndef LCD1602_H
#define LCD1602_H

#include <stdint.h>

#define LCD1602_ADDR      0x27
#define LCD1602_BACKLIGHT 0x08
#define LCD1602_ENABLE    0x04
#define LCD1602_CHR       1 
#define LCD1602_CMD       0 
#define LCD1602_LINE_1    0x80 
#define LCD1602_LINE_2    0xC0 

void lcd1602_setup(uint32_t i2c);
void lcd1602_print_string(uint32_t i2c, uint8_t line, const char *msg);

#endif // LCD1602_H

