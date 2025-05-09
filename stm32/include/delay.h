#ifndef DELAY_H
#define DELAY_H

#include <libopencm3/stm32/f4/rcc.h>
#include <libopencm3/cm3/dwt.h>

uint32_t dwt_setup(void);
void dwt_delay_us(uint32_t us);
void dwt_delay_ms(uint32_t ms);

#endif // DELAY_H
