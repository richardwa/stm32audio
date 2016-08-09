#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "oscillator.h"


#define PERIOD  100
#define SAMPLES (2*PERIOD)

//test function for reference
#define PI 3.14159265

int32_t sine_actual(int32_t period, uint32_t phase)
{
    double x = phase % period;
    return 100*sin(x*PI*2/period);
}

int main()
{
  int i;
  for (i = 0; i < SAMPLES; i++) {
//    printf("%d,", sawtooth(500,i));
//    printf("%d,", triangle(period,i));
//    printf("%d,", square(period,i));
//    printf("%d,", sine_actual(PERIOD,i));
    printf("%d,", sine(PERIOD,i));
    printf("\n");
  }
  return 0;
}
