#ifndef SEND_LOG
#define SEND_LOG

#include <libopencm3/stm32/usart.h>

void usart_setup(void);
void usart_transmit(const char *str);

#endif // SEND_LOG
