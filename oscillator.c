#include "oscillator.h"
#include "debug.h"

#define LEVEL 0xFFFF

uint16_t sawtooth(uint32_t period, uint32_t phase) 
{
  dprintf("sawtooth %d %d", period, phase);
  return (phase % period) * LEVEL / period;
}

uint16_t triangle(uint32_t period, uint32_t phase) 
{
  uint32_t current = phase % period;
  uint32_t halfperiod = period / 2;
  if (current <= halfperiod) {
    return current * 2 * LEVEL / period;
  } else {
    return (period - current) * 2 * LEVEL / period;
  }
}

uint16_t square(uint32_t period, uint32_t phase) 
{
  if (phase < period/2)
    return LEVEL;
  else
    return 0;
}


//taylor series cos approx: 1 - x^2/2!  + x^4/4!
uint16_t sine(uint32_t period, uint32_t phase) 
{
  return 0;
}

//integer based exponentiation
int ipow(int base, int ex)
{
    int result = 1;
    while (ex)
    {
        if (ex & 1)
            result *= base;
        ex >>= 1;
        base *= base;
    }

    return result;
}

