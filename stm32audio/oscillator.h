#ifndef OSCILLATOR_H
#define OSCILLATOR_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#define OSCILLATOR_LEVEL 30000

int32_t sawtooth(int32_t period, uint32_t phase);
int32_t triangle(int32_t period, uint32_t phase);
int32_t square(int32_t period, uint32_t phase);
int32_t sine(int32_t period, uint32_t phase);
int32_t test(int32_t period, uint32_t phase);

#ifdef __cplusplus
}
#endif
#endif
