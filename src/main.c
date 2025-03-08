#include <libopencm3/stm32/f4/rcc.h>
#include <libopencm3/stm32/f4/gpio.h>
#include <libopencm3/stm32/f4/i2c.h>
#include <libopencm3/stm32/f4/adc.h>
#include <stdio.h>

#include "i2c_init.h"
#include "lcd1602.h"
#include "soil.h"
#include <libopencm3/stm32/f4/rcc.h>
#include <libopencm3/stm32/f4/adc.h>
#include <libopencm3/stm32/f4/gpio.h>

static void clock_setup(void) {
    rcc_osc_on(RCC_HSE);
    rcc_wait_for_osc_ready(RCC_HSE);
    rcc_set_sysclk_source(RCC_HSE);

    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_I2C1);
}

void adc_setup(void) {
    rcc_periph_clock_enable(RCC_ADC1);
    rcc_periph_clock_enable(RCC_GPIOA);

    gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO0);

    adc_power_off(ADC1);
    adc_disable_scan_mode(ADC1);
    adc_set_single_conversion_mode(ADC1);
   
    uint8_t channels[1] = { ADC_CHANNEL0 };
    adc_set_regular_sequence(ADC1, 1, channels);

    adc_set_sample_time(ADC1, ADC_CHANNEL0, ADC_SMPR_SMP_28CYC);
    adc_power_on(ADC1);
    for (volatile int i = 0; i < 100000; i++);
}

int main(void) {
    clock_setup();
    gpio_setup();
    i2c_setup();
    adc_setup();

    lcd1602_setup(I2C1);

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
        if (wet_or_dry == 0) {
            lcd1602_print_string(I2C1, LCD1602_LINE_1, "It's wet!!!");
            lcd1602_print_string(I2C1, LCD1602_LINE_2, third);
        } else {
            lcd1602_print_string(I2C1, LCD1602_LINE_1, "It's dry!!!");
            lcd1602_print_string(I2C1, LCD1602_LINE_2, third);
        }
    }
}

