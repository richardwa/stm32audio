#include <stdio.h>
#include <stdint.h>

#include "synth.h"
#include "notes.h"


#define SAMPLE_RATE 44100
#define TUNING 440

int main()
{
  int i;
  uint32_t playbuffer;
  notes_init(SAMPLE_RATE, TUNING);
  synth_set_oscillator(0);
  synth_note_on(69, 255);
  for (i = 0; i < 1000; i++) {
    playbuffer = synth_get_wave(i);
    printf("%d ", playbuffer);
  }
  printf("end\n");

  return 0;
}
