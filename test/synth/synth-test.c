#include <stdio.h>
#include <stdint.h>

#include "synth.h"
#include "notes.h"


#define SAMPLE_RATE 31250
#define TUNING 440

int main()
{
  int i;
  uint32_t playbuffer;
  notes_init(SAMPLE_RATE, TUNING);
  synth_note_on(69, 255);
  
  //synth_note_on(75, 255);
  for (i = 0; i < 20000; i++) {
    playbuffer = synth_get_wave(i);
    printf("%d\n", playbuffer);
    if (i%32 == 0){
        synth_env_update();
    }
    if (i == 1400000){
        synth_note_off(69);
    }
  }
  printf("\n");

  return 0;
}
