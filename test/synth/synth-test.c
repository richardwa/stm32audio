#include <stdio.h>
#include <stdint.h>

#include "synth.h"
#include "notes.h"


#define SAMPLE_RATE 31250
#define TUNING 440

int main()
{
  int i;
  int16_t playbuffer;
  notes_init(SAMPLE_RATE, TUNING);
//  synth_note_on(69, 255);
//  synth_note_on(76, 255);
  synth_note_on(81, 255);
  
  //synth_note_on(75, 255);
  for (i = 0; i < 4000; i++) {

    playbuffer = synth_get_wave(i);
    if (i>2000){
        printf("%d\n", playbuffer);
    }
  }
  printf("\n");

  return 0;
}
