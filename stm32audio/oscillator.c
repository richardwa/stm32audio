#include "oscillator.h"
#include "debug.h"


int16_t sawtooth(uint16_t vol, uint16_t period, uint32_t phase)
{
  dprintf("sawtooth %d %d", period, phase);
  return (phase % period) * vol / period - vol / 2;
}

int16_t triangle(uint16_t vol, uint16_t period, uint32_t phase)
{
  uint32_t x = phase % period;
  uint32_t halfperiod = period / 2;
  if (x < halfperiod) {
    return x * 2 * vol / period - vol / 2;
  } else {
    return (period - x) * 2 * vol / period - vol / 2;
  }
}
volatile int16_t result;
int16_t test(uint16_t vol, uint16_t period, uint32_t phase)
{
  int8_t i;
  result = sine2(vol/period, period * 7 / 5, phase);
  for (i=0;i<10;i++){
    result = sine2(vol/period, period * 7 / 5, result);
  }
  
  return sine2(vol, period ,  phase);
}

//square wave is 2 twice as loud, so reducing its peak more
int16_t square(uint16_t vol, uint16_t period, uint32_t phase)
{
  if (phase % period < period / 2)
    return vol / 4;
  else
    return -vol / 4;
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
// scale x, x->x/A
// scale y, f->A*f
//   f(x) = -B(a(x/A)^3 + b(x/A)^2 + c(x/A))
//
// implemented with only integer calculations and taking care to not overflow int32

//since our sine cubic coeffcients are floats, we need to incorporate LEVEL into it
//update these as level is updated
#define SIN_ADJ 10000
#define SIN_A  -4292  // -0.4292036732
#define SIN_B  -1416  // -0.1415926536
#define SIN_C  15708  //  1.570796327
#define SIN_FUNC(v,x,p) \
  ( SIN_A * 4 * 4 * v / SIN_ADJ * 4 * x / p * x / p * x / p \
  + SIN_B * 4 * 4 * v / SIN_ADJ * x / p * x / p \
  + SIN_C * 4 * v / SIN_ADJ * x / p )
  int16_t sine(uint16_t vol, uint16_t period, uint32_t phase)
{

  int32_t x = phase % period;
  uint8_t section = x * 4 / period;

  switch (section) {
    case 0:  //the is the fist quarter of the the wave
      return SIN_FUNC(vol,x,period);
    case 1:
      x =  period / 2 - x; // mirror flip into first quater
      return SIN_FUNC(vol,x,period);
    case 2:
      x =  x - period / 2; // translate to first quater
      //negative of first half
      return -SIN_FUNC(vol,x,period);
    case 3:
      x =  x - period / 2; // translate to second quater
      x =  period / 2 - x; // mirror flip into first quater
      //negative of first half
      return -SIN_FUNC(vol,x,period);
  };
}

//quadratic aprroximation of sine
// base function:
//  f(x)  =  -x^2 + 1
//  this is really a cosine, but doesn't matter for us
// scale x, x->x/A
// scale y, f->A*f
//   f(x) = B(-(x/A)^2 + 1)
#define SIN2_FUNC(v,x,p) (v  -  v * x * x * 16 / p / p)
  int16_t sine2(uint16_t vol, uint16_t period, uint32_t phase)
{

  int32_t v = vol/2;
  int32_t x = phase % period;
  uint8_t section = x * 4 / period;
  switch (section) {
    case 0:  //the is the fist quarter of the the wave
      return SIN2_FUNC(v,x,period);
    case 1:
      x =  period / 2 - x; // mirror flip into first quater
      return -SIN2_FUNC(v,x,period);
    case 2:
      x =  x - period / 2; // translate to first quater
      //negative of first half
      return -SIN2_FUNC(v,x,period);
    case 3:
      x =  x - period / 2; // translate to second quater
      x =  period / 2 - x; // mirror flip into first quater
      return SIN2_FUNC(v,x,period);
  };

}

