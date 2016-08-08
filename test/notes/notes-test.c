#include <stdio.h>
#include <stdint.h>

#include "notes.h"


int main()
{
  notes_init(44000,440);
  int i;
  for (i = 0; i < 128; i++) {
    printf("%d,", note_periods[i]);
    printf("\n");
  }
  return 0;
}
