#include <stdio.h>
#include <stdint.h>
#include <errno.h>

#include "synth.h"
#include "notes.h"
#include "wavfile.h"


#define SAMPLE_RATE 44100
#define DURATION 10
#define SAMPLE_SIZE (SAMPLE_RATE * DURATION)
#define TUNING 440

int main()
{
  int i;
  int16_t playbuffer[SAMPLE_SIZE];
  notes_init(SAMPLE_RATE, TUNING);
  synth_note_on(69, 128);
  
  //synth_note_on(75, 255);
  for (i = 0; i < SAMPLE_SIZE ; i++) {
    playbuffer[i] = (synth_get_wave(i) / 256) << 8;
    //printf("%d\n", playbuffer[i]);
    if (i == (SAMPLE_SIZE * 7/8)){
        synth_note_off(69);
    }
  }
  printf("\n");

  FILE * f = wavfile_open("/tmp/sound.wav", SAMPLE_RATE);
  if(!f) {
    printf("couldn't open sound.wav for writing: %s",strerror(errno));
    return 1;
  }

  wavfile_write(f,playbuffer,SAMPLE_SIZE);
  wavfile_close(f);

  return 0;
}
