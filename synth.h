#ifndef SYNTH_H 
#define SYNTH_H

int32_t synth_get_wave(uint32_t tick);
void synth_note_on(uint8_t index, uint8_t velocity);
void synth_note_off(uint8_t index);
void synth_set_oscillator(uint8_t index);

#endif
