#ifndef SYNTH_H 
#define SYNTH_H
#ifdef __cplusplus
extern "C" {
#endif


int32_t synth_get_wave(uint32_t tick);
void synth_note_on(uint8_t index, uint8_t velocity);
void synth_note_off(uint8_t index);

#ifdef __cplusplus
}
#endif
#endif
