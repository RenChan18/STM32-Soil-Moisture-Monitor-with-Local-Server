#include <stdio.h>

#include "lcd1602.h"
#include "app_log.h"
//#include "delay.h"


static void start_periphery(void) {
    clock_setup();
    gpio_setup();
    usart_setup();
    i2c_setup();
    dwt_setup();
    adc_setup();

    //lcd1602_setup(I2C1);
}


int main(void) {
    start_periphery();
    start_app();
    while(1){};
}

