#ifndef OSCILLATOR_H
#define OSCILLATOR_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

int16_t sawtooth(uint16_t vol, uint16_t period, uint32_t phase);
int16_t triangle(uint16_t vol, uint16_t period, uint32_t phase);
int16_t square(uint16_t vol, uint16_t period, uint32_t phase);
int16_t sine3(uint16_t vol, uint16_t period, uint32_t phase);
int16_t sine2(uint16_t vol, uint16_t period, uint32_t phase);
int16_t test(uint16_t vol, uint16_t period, uint32_t phase);

#ifdef __cplusplus
}
#endif
#endif
