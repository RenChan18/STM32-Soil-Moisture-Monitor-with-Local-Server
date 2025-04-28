#ifndef PERIPH_INIT_H
#define PERIPH_INIT_H

#include <libopencm3/stm32/f4/rcc.h>
#include <libopencm3/stm32/f4/gpio.h>
#include <libopencm3/stm32/f4/usart.h>
#include <libopencm3/stm32/f4/adc.h>
#include <libopencm3/stm32/f4/i2c.h>
#include <libopencm3/cm3/nvic.h>

#include "delay.h"

#define GREEN_LED GPIO12
#define BAUD_RATE 9600

void clock_setup(void);
void gpio_setup(void);
void adc_setup(void);
void i2c_setup(void); 
void usart_setup(void); 

#endif // PERIPH_INIT_H

