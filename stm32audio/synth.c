#include <stdint.h>
#include "oscillator.h"
#include "voice.h"
#include "notes.h"
#include "debug.h"

//playing at 22 micros period, we need the buffer to cover at least 0.05 seconds for the low notes
//highest pitch alternating +/- which comes to 20khz
//lowest pitch is all one period in 0.05

#define POLYPHONY 9

const struct Voice model = {
  .volume = 1,
  .ticks = 0,
  .oscillator = &square,
  .adsr_phase = 1,
  .attack = 50,
  .decay = 200,
  .sustain = -1,
  .rel = 50
};

int8_t noteMap[128] = { -1}; //holds index of activeVoices
struct Voice voices[POLYPHONY];

//query and sum voices for current wave amplitude
int16_t synth_get_wave(uint32_t tick)
{
  int32_t temp = 0;
  uint8_t i;
  for ( i = 0; i < POLYPHONY; i++) {
    struct Voice *v = &voices[i];
    if (v->volume > 0) {
      temp += (v->oscillator)(v->volume, v->period, tick);   //invoke oscillator
    }
  }
  
  if (temp > INT16_MAX){
    return INT16_MAX;
  } else if (temp < INT16_MIN){
    return INT16_MIN;
  } else{
    return temp;
  }
}

void synth_env_update() {
  uint8_t i;
  for ( i = 0; i < POLYPHONY; i++) {
    voice_update(&voices[i]);
  }
}

void synth_note_on(uint8_t index, uint8_t velocity)
{
  uint8_t i;
  for (i = 0; i < POLYPHONY; i++) {
    struct Voice *v = &voices[i];
    dprintf("finding slot %d %d\n", i, v->volume);
    if (v->volume == 0 || v->period == note_periods[index]) {
      noteMap[index] = i; //save index for note off to find us
      dprintf("setting voice %d\n", i);
      *v = model;
      v->velocity = velocity * 0xFF;
      v->period = note_periods[index];
      dprintf("voice set %d\n", v->volume);
      break;
    }
  }
}

void synth_note_off(uint8_t index)
{
  int8_t voiceIndex = noteMap[index];
  if (voiceIndex >= 0) {
    voices[voiceIndex].adsr_phase = 4;
  }
}
