#include "send_log.h"

void usart_setup(void) {
    usart_set_baudrate(USART2, 115200);
    usart_set_databits(USART2, 8);
    usart_set_stopbits(USART2, USART_STOPBITS_1);
    usart_set_parity(USART2, USART_PARITY_NONE);
    usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
    usart_set_mode(USART2, USART_MODE_TX);
    usart_enable(USART2);
}

void usart_transmit(const char *str) {
    while (*str) {
        usart_send_blocking(USART2, *str++);
    }
    usart_send_blocking(USART2, '\r');
    usart_send_blocking(USART2, '\n');
}
