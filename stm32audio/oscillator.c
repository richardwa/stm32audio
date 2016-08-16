#include "oscillator.h"
#include "debug.h"

//level should not go beyond signed int16
#define LEVEL OSCILLATOR_LEVEL

//since our sine cubic coeffcients are floats, we need to incorporate LEVEL into it
//update these as level is updated
#define SIN_CUBIC_A -429 // -0.4292036732
#define SIN_CUBIC_B -141 // -0.1415926536
#define SIN_CUBIC_C 1570 //  1.570796327

int32_t sawtooth(int32_t period, uint32_t phase)
{
  dprintf("sawtooth %d %d", period, phase);
  return (phase % period) * LEVEL / period - LEVEL / 2;
}

int32_t triangle(int32_t period, uint32_t phase)
{
  uint32_t x = phase % period;
  uint32_t halfperiod = period / 2;
  if (x < halfperiod) {
    return x * 2 * LEVEL / period - LEVEL / 2;
  } else {
    return (period - x) * 2 * LEVEL / period - LEVEL / 2;
  }
}
int32_t test(int32_t period, uint32_t phase)
{
  return triangle(period, triangle(period * 7/5, phase) * period / LEVEL  + phase);
}

int32_t square(int32_t period, uint32_t phase)
{
  if (phase % period < period / 2)
    return LEVEL;
  else
    return -LEVEL;
}


//cubic aprroximation of sine
// base function:
//  f(x)  =  ax^3 +  bx^2 + cx
//  f'(x) = 3ax^2 + 2bx   + c
//
// desired conditions, matching sin(pi/2 *x):
//  f(0) = 0, f(1) = 1, f'(0) = pi/2 , f'(1) = 0
// solve the system we get:
//  a = pi/2 - 2
//  b = -pi + 3
//  c = pi/2
//
// scale x, x->x/B
// scale y, f->A*f
//   f(x) = -B(a(x/A)^3 + b(x/A)^2 + c(x/A))
//
// implemented with only integer calculations and taking care to not overflow int32
int32_t sine(int32_t period, uint32_t phase)
{

  int32_t x = phase % period;
  uint8_t section = x * 4 / period;

  switch (section) {
    case 0:  //the is the fist quarter of the the wave
      return SIN_CUBIC_A * 4 * 4 * 4 * x / period * x / period * x / period
             + SIN_CUBIC_B * 4 * 4 * x / period * x / period
             + SIN_CUBIC_C * 4 * x / period;
    case 1:
      x =  period / 2 - x; // mirror flip into first quater
      return SIN_CUBIC_A * 4 * 4 * 4 * x / period * x / period * x / period
             + SIN_CUBIC_B * 4 * 4 * x / period * x / period
             + SIN_CUBIC_C * 4 * x / period;
    case 2:
      x =  x - period / 2; // translate to first quater
      //negative of first half
      return -1 * SIN_CUBIC_A * 4 * 4 * 4 * x / period * x / period * x / period
             - SIN_CUBIC_B * 4 * 4 * x / period * x / period
             - SIN_CUBIC_C * 4 * x / period;
    case 3:
      x =  x - period / 2; // translate to second quater
      x =  period / 2 - x; // mirror flip into first quater
      //negative of first half
      return -1 * SIN_CUBIC_A * 4 * 4 * 4 * x / period * x / period * x / period
             - SIN_CUBIC_B * 4 * 4 * x / period * x / period
             - SIN_CUBIC_C * 4 * x / period;

  };
}

