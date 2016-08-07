#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <stdint.h>

uint16_t sawtooth(uint32_t period, uint32_t phase);
uint16_t triangle(uint32_t period, uint32_t phase);
uint16_t square(uint32_t period, uint32_t phase);
//uint16_t sine(uint32_t period, uint32_t phase);

#endif
