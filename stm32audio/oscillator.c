#include "oscillator.h"
#include "debug.h"

#define LEVEL 500

int32_t sawtooth(int32_t period, uint32_t phase)
{
  dprintf("sawtooth %d %d", period, phase);
  return (phase % period) * LEVEL / period - LEVEL/2;
}

int32_t triangle(int32_t period, uint32_t phase)
{
  uint32_t x = phase % period;
  uint32_t halfperiod = period / 2;
  if (x < halfperiod) {
    return x * 2 * LEVEL / period - LEVEL/2;
  } else {
    return (period - x) * 2 * LEVEL / period - LEVEL/2;
  }
}

int32_t square(int32_t period, uint32_t phase)
{
  if (phase % period < period / 2)
    return LEVEL;
  else
    return -LEVEL;
}



//taylor series sine approx at zero : x - x^3/3!  + x^5/5! ...
//we will just take first 2 terms
//  first lets normalize sine to period and level
//    f(t) = (LEVEL/2) sine(2PIx/period)
//  substitute
//         = (LEVEL/2) (x2PI/period - (x2PI/period)^3/6)
//  distribute
//         = (LEVEL/2) (x2PI/period) - (LEVEL/2)(x2PI/period)^3/6)
//  simplify
//         = (PILEVEL/period)x - (LEVEL/2)(2PI/period)^3 (x^3)/6
//         = (PILEVEL/period)x - (LEVEL/12)(2PI)^3 (x^3)/(period^3)
//  finally we get (in code)
//  x * PI_INT * LEVEL / period - (LEVEL / 12) * cube(2 * PI_INT) * cube(x) / cube(period);
//  
//  the useful range of the approx is between -PI and PI, we will do some reflecting and translating to 
//
#define PI_INT 3
#define cube(x) ((x)*(x)*(x))
#define square(x) ((x)*(x))
int32_t sine(int32_t period, uint32_t phase)
{

  //phase shift now goes for -period/2 to period/2
  int32_t x = phase % period - period / 2;

  //mirror and reuse the good portion of the series
  if (x > period / 4) {
    x =  period / 2 - x;
  } else if (x < -1 * period / 4) {
    x = -1 * (period / 2 + x);
  }

  //correct the phase after all the shifting, take the negative
  return x * PI_INT * -LEVEL / period + (LEVEL / 12) * cube(2 * PI_INT) * cube(x) / cube(period) ;

}

int32_t sine2(int32_t period, uint32_t phase)
{

  int32_t x = phase % period - period/2;

  if (x > period / 4) {
    x =  period / 4 - x;
  } else if (x < -1 * period / 4) {
    x = -1 * (period / 4 + x);
  }
  
  
  return  (-1 * LEVEL * cube(4)*x/period*x/period + LEVEL * square(4) *x/period + LEVEL * 4 )*x/period;

}

