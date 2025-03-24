#include "lcd1602.h"

static void lcd1602_write_byte(uint32_t i2c, uint8_t byte, uint8_t flag) {
    uint8_t transaction[6] = {0};
    transaction[0] = (flag | (byte & 0xF0) | LCD1602_BACKLIGHT);
    transaction[1] = ((flag | (byte & 0xF0) | LCD1602_BACKLIGHT) | LCD1602_ENABLE);
    transaction[2] = ((flag | (byte & 0xF0) | LCD1602_BACKLIGHT) & ~LCD1602_ENABLE);
    transaction[3] = (flag | ((byte << 4) & 0xF0) | LCD1602_BACKLIGHT);
    transaction[4] = ((flag | ((byte << 4) & 0xF0) | LCD1602_BACKLIGHT) | LCD1602_ENABLE);
    transaction[5] = ((flag | ((byte << 4) & 0xF0) | LCD1602_BACKLIGHT) & ~LCD1602_ENABLE);

    for(uint8_t i = 0; i < 6; i++){
        i2c_transfer7(i2c, LCD1602_ADDR, &transaction[i], 1, NULL, 0);
    }
}

void lcd1602_setup(uint32_t i2c) {
    lcd1602_write_byte(i2c, 0x33, LCD1602_CMD); // 110011 Initialise
    lcd1602_write_byte(i2c, 0x32, LCD1602_CMD); // 110010 Initialise
    lcd1602_write_byte(i2c, 0x06, LCD1602_CMD); // 000110 Cursor move direction
    lcd1602_write_byte(i2c, 0x0C, LCD1602_CMD); // 001100 Display On,Cursor Off, Blink Off
    lcd1602_write_byte(i2c, 0x28, LCD1602_CMD); // 101000 Data length, number of lines, font size
    lcd1602_write_byte(i2c, 0x01, LCD1602_CMD); // 000001 Clear display
}

void lcd1602_print_string(uint32_t i2c, uint8_t line, const char *msg) {
    lcd1602_write_byte(i2c, line, LCD1602_CMD);
    while (*msg) {
        lcd1602_write_byte(i2c, (uint8_t)*msg++, LCD1602_CHR);
    }
}

