#include <stdint.h>
#include "oscillator.h"
#include "notes.h"
#include "debug.h"

//playing at 22 micros period, we need the buffer to cover at least 0.05 seconds for the low notes
//highest pitch alternating +/- which comes to 20khz
//lowest pitch is all one period in 0.05

#define POLYPHONY 4
#define MAX_VOICE_VOLUME 0xFF

struct Voice {
  //wave characteristics
  uint8_t volume;   //loudness -- 0 indicates voice is off
  uint16_t period; //pitch 

  //adsr envelope
  uint32_t ticks;  
  uint16_t attack; //is a duration
  uint16_t decay;  //is a duration
  uint8_t sustain; //is a level
  uint16_t rel;    //is a duration

  int32_t (*oscillator)(int32_t period, uint32_t phase);
};

struct Voice model = { 
  .volume = 1,
  .oscillator = &test
};

int8_t noteMap[128] = {-1}; //holds index of activeVoices
struct Voice voices[POLYPHONY];


//query and sum voices for current wave amplitude
int32_t synth_get_wave(uint32_t tick) 
{
  int32_t temp = 0;
  uint8_t i;
  for ( i = 0; i < POLYPHONY; i++) {
    struct Voice *v = &voices[i];
    if (v->volume > 0) {
      temp += 
        ((v->oscillator)(v->period, tick)   //invoke oscillator
          * (v->volume / MAX_VOICE_VOLUME)  //apply volume
        ) / POLYPHONY ;                     //reduce loudness by number of voices
        
      v->ticks++;
    }
  }
  return temp;
}

void synth_set_oscillator(uint8_t index)
{
  model.oscillator = &sine; 
}

void synth_note_on(uint8_t index, uint8_t velocity) 
{
  uint8_t i;
  for (i = 0; i < POLYPHONY; i++) {
    struct Voice *v = &voices[i];
    dprintf("finding slot %d %d\n",i,v->volume);
    if (v->volume == 0) {
      noteMap[index] = i; //save index for note off to find us
      dprintf("setting voice %d\n",i);
      *v = model;
      v->volume = velocity;
      v->period = note_periods[index];
      dprintf("voice set %d\n",v->volume);
      break;
    }
  }
}

void synth_note_off(uint8_t index) 
{
  int8_t voiceIndex = noteMap[index];
  if (voiceIndex >= 0){
    voices[voiceIndex].volume = 0;
  }
}
