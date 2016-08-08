#include <stdio.h>
#include <stdint.h>

#include "oscillator.h"


#define period  100
#define samples (2*period)
int main()
{
  int i;
  for (i = 0; i < samples; i++) {
//    printf("%d,", sawtooth(500,i));
//    printf("%d,", triangle(period,i));
//    printf("%d,", square(period,i));
    printf("%d,", sine(period,i));
    printf("\n");
  }
  return 0;
}
