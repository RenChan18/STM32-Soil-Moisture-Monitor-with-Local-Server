#include "delay.h"

uint32_t dwt_setup(void) {
    if (!dwt_enable_cycle_counter()) {
        return 1; 
    }
    return 0; 
}

void dwt_delay_us(uint32_t us) {
    uint32_t initial_tics = dwt_read_cycle_counter();
    uint32_t cycles_tics = us * (rcc_ahb_frequency/1000000);
    while ((uint32_t)(dwt_read_cycle_counter() - initial_tics) < cycles_tics);
}

void dwt_delay_ms(uint32_t ms) {
    dwt_delay_us(ms * 1000);
}
