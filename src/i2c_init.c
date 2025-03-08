#include <libopencm3/stm32/f4/rcc.h>
#include <libopencm3/stm32/f4/gpio.h>
#include <libopencm3/stm32/f4/i2c.h>

#include "lcd1602.h"
void gpio_setup(void) {
    gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO10);
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO8 | GPIO9);
    gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO8 | GPIO9);
    gpio_set_af(GPIOB, GPIO_AF4, GPIO8 | GPIO9);
}

void i2c_setup(void) {
    i2c_peripheral_disable(I2C1);
    i2c_set_own_7bit_slave_address(I2C1, 0x00);
    i2c_set_speed(I2C1, i2c_speed_sm_100k, 50);
    i2c_peripheral_enable(I2C1);
}

