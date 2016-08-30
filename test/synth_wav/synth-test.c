#include <stdio.h>
#include <stdint.h>
#include <errno.h>

#include "synth.h"
#include "notes.h"
#include "wavfile.h"


#define SAMPLE_RATE 31250
#define DURATION 10
#define SAMPLE_SIZE (SAMPLE_RATE * DURATION)
#define TUNING 440

//uint8_t chord[] = {69, 73, 76};
uint8_t chord[] = {69,73,76,81,85,88,93,97,100};
void chorus(int8_t vol) {
  uint8_t i;
  for (i = 0; i < sizeof(chord) / sizeof(chord[0]); i++) {
    synth_note_on(chord[i], vol);
  }
}

int main()
{
  int i;
  int16_t playbuffer[SAMPLE_SIZE];
  notes_init(SAMPLE_RATE, TUNING);
  chorus(255);
  
  //synth_note_on(75, 255);
  for (i = 0; i < SAMPLE_SIZE ; i++) {
    playbuffer[i] = synth_get_wave(i); 
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
