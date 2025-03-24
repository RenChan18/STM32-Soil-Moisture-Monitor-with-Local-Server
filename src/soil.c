#include "soil.h"

#define MAX_DRYNESS 4095

uint8_t read_soil_digital(void) {
    return gpio_get(GPIOB, GPIO10) ? 1 : 0;
}

uint16_t read_soil_analog(void) {
    adc_start_conversion_regular(ADC1);
    while (!(adc_eoc(ADC1)));
    return adc_read_regular(ADC1);
}

uint16_t to_percent(uint16_t num) {
    return 100 - (num * 100) / MAX_DRYNESS;
}

