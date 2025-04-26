#include <stdio.h>

#include "lcd1602.h"
#include "app_log.h"



static void start_periphery() {
    clock_setup();
    gpio_setup();
    usart_setup();
    i2c_setup();
    adc_setup();
    dwt_setup();

    lcd1602_setup(I2C1);
}


int main(void) {
    volatile uint8_t wet_or_dry = 0;
    volatile uint16_t humidity_level = 0;
    char str[20];
    const char fstr[] = "Humidity is ";
    char third[512];

    while (1) {
        wet_or_dry = read_soil_digital();
        humidity_level = to_percent(read_soil_analog());
        sprintf(str, "%u", humidity_level);
        sprintf(third, "%s%s", fstr, str);
        sprintf(third, "%s%s", third, "%");
        if (wet_or_dry == 1) {
            lcd1602_print_string(I2C1, LCD1602_LINE_1, "It's wet!!!");
            lcd1602_print_string(I2C1, LCD1602_LINE_2, third);
        } else {
            lcd1602_print_string(I2C1, LCD1602_LINE_1, "It's dry!!!");
            lcd1602_print_string(I2C1, LCD1602_LINE_2, third);
        }
        usart_transmit(third);
        dwt_delay_ms(10000);
    }
}

