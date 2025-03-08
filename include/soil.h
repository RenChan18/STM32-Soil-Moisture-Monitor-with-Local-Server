#ifndef SOIL_H
#define SOIL_H

#include <stdint.h>

uint8_t read_soil_digital(void);
uint16_t read_soil_analog(void);
uint16_t to_percent(uint16_t num);

#endif // SOIL_H

