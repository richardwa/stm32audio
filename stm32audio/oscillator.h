#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <stdint.h>

int32_t sawtooth(int32_t period, uint32_t phase);
int32_t triangle(int32_t period, uint32_t phase);
int32_t square(int32_t period, uint32_t phase);
int32_t sine(int32_t period, uint32_t phase);

#endif
