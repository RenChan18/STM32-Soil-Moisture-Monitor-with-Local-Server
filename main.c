#include <libopencm3/stm32/f4/rcc.h>
#include <libopencm3/stm32/f4/gpio.h>
#include <libopencm3/stm32/f4/i2c.h>
#include <libopencm3/stm32/f4/adc.h>
#include <stdio.h>

#define LCD1602_ADDR 0x27
#define LCD1602_BACKLIGHT 0x08
#define LCD1602_ENABLE 0x04
#define LCD1602_WIDTH 20   
#define LCD1602_CHR  1 
#define LCD1602_CMD  0 
#define LCD1602_LINE_1  0x80 
#define LCD1602_LINE_2  0xC0 

#define MAX_DRYNESS 4095
void gpio_setup(void) {
    gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO10);
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO8 | GPIO9);
    gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO8 | GPIO9);
    gpio_set_af(GPIOB, GPIO_AF4, GPIO8 | GPIO9);
}

static void clock_setup(void) {
    rcc_osc_on(RCC_HSE);
    rcc_wait_for_osc_ready(RCC_HSE);
    rcc_set_sysclk_source(RCC_HSE);

    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_I2C1);
}

void i2c_setup(void) {
    i2c_peripheral_disable(I2C1);
    i2c_set_own_7bit_slave_address(I2C1, 0x00);
    i2c_set_speed(I2C1, i2c_speed_sm_100k, 50);
    i2c_peripheral_enable(I2C1);
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
   //adc_enable_temperature_sensor();
   adc_power_on(ADC1);
   for (volatile int i = 0; i < 100000; i++);
}

static void lcd1602_write_byte(uint32_t i2c, uint8_t byte, uint8_t flag){
    uint8_t transaction[6] = {0};
    transaction[0] = (flag | (byte & 0xF0) | LCD1602_BACKLIGHT);
    transaction[1] = ((flag | (byte & 0xF0) | LCD1602_BACKLIGHT) | LCD1602_ENABLE);
    transaction[2] = ((flag | (byte & 0xF0) | LCD1602_BACKLIGHT) & ~LCD1602_ENABLE);
    transaction[3] = (flag | ((byte<<4) & 0xF0) | LCD1602_BACKLIGHT);
    transaction[4] = ((flag | ((byte<<4) & 0xF0) | LCD1602_BACKLIGHT) | LCD1602_ENABLE );
    transaction[5] = ((flag | ((byte<<4) & 0xF0) | LCD1602_BACKLIGHT) & ~LCD1602_ENABLE);

    for(uint8_t i = 0; i < 6; i++){
        i2c_transfer7(i2c, LCD1602_ADDR, &transaction[i], 1, NULL, 0);
    }
}

static void lcd1602_setup(uint32_t i2c){
    lcd1602_write_byte(i2c, 0x33, LCD1602_CMD); // 110011 Initialise
    lcd1602_write_byte(i2c, 0x32, LCD1602_CMD); // 110010 Initialise
    lcd1602_write_byte(i2c, 0x06, LCD1602_CMD); // 000110 Cursor move direction
    lcd1602_write_byte(i2c, 0x0C, LCD1602_CMD); // 001100 Display On,Cursor Off, Blink Off
    lcd1602_write_byte(i2c, 0x28, LCD1602_CMD); // 101000 Data length, number of lines, font size
    lcd1602_write_byte(i2c, 0x01, LCD1602_CMD); // 000001 Clear display
}

static void lcd1602_print_string(uint32_t i2c, uint8_t line, const char *msg){
    lcd1602_write_byte(i2c, line, LCD1602_CMD);
    while (*msg) {
        lcd1602_write_byte(i2c, (uint8_t)*msg++, LCD1602_CHR);
    }
}

uint8_t read_soil_digital(void){
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

int main() {
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
        }
        else {
            lcd1602_print_string(I2C1, LCD1602_LINE_1, "It's dry!!!");
            lcd1602_print_string(I2C1, LCD1602_LINE_2, third);
        }
    }
}



