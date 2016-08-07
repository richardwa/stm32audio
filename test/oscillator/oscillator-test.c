#include <stdio.h>
#include <stdint.h>

#include "oscillator.h"


#define period 20
int main()
{
  int i;
  uint16_t playbuffer;
  for (i = 0; i < 100; i++) {
//    printf("%d,", sawtooth(500,i));
    printf("%d,", triangle(period,i));
//    printf("%d,", square(period,i));
    printf("\n");
  }
  return 0;
}
