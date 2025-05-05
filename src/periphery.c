#include "periphery.h"

void clock_setup(void) {
    rcc_osc_on(RCC_HSE);
    rcc_wait_for_osc_ready(RCC_HSE);
    rcc_set_sysclk_source(RCC_HSE);

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOD);

    rcc_periph_clock_enable(RCC_I2C1);
    rcc_periph_clock_enable(RCC_USART2);
    rcc_periph_clock_enable(RCC_ADC1);
}

void adc_setup(void) {
    adc_power_off(ADC1);
    adc_disable_scan_mode(ADC1);
    adc_set_single_conversion_mode(ADC1);
   
    uint8_t channels[1] = { ADC_CHANNEL0 };
    adc_set_regular_sequence(ADC1, 1, channels);

    adc_set_sample_time(ADC1, ADC_CHANNEL0, ADC_SMPR_SMP_28CYC);
    adc_power_on(ADC1);
    dwt_delay_us(10);
}

void gpio_setup(void) {
    /* i2c */
    gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO10);
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO8 | GPIO9);
    gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO8 | GPIO9);
    gpio_set_af(GPIOB, GPIO_AF4, GPIO8 | GPIO9);

    /* usart */
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2 | GPIO3);
    gpio_set_af(GPIOA, GPIO_AF7, GPIO2 | GPIO3);

    /* green led */
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GREEN_LED);
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, ORANGE_LED);
    
    /* soil scan*/
    gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO0);
}   


void i2c_setup(void) {
    i2c_peripheral_disable(I2C1);
    i2c_set_own_7bit_slave_address(I2C1, 0x00);
    i2c_set_speed(I2C1, i2c_speed_sm_100k, 50);
    i2c_peripheral_enable(I2C1);
}

void usart_setup(void) {
    usart_set_baudrate(USART2, BAUD_RATE);
    usart_set_databits(USART2, 8);
    usart_set_stopbits(USART2, USART_STOPBITS_1);
    usart_set_parity(USART2, USART_PARITY_NONE);
    usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
    usart_set_mode(USART2, USART_MODE_TX_RX);
    usart_enable_rx_interrupt(USART2);

    nvic_enable_irq(NVIC_USART2_IRQ);
    usart_enable(USART2);
}


