#ifndef VOICE_H
#define VOICE_H
#ifdef __cplusplus
extern "C" {
#endif

struct Voice {
  //wave characteristics
  uint8_t volume;   //loudness -- 0 indicates voice is off
  uint8_t velocity;  //how hard was it hit
  uint16_t period; //pitch
  int32_t (*oscillator)(int32_t period, uint32_t phase);

  //adsr envelope
  uint32_t ticks;
  uint8_t adsr_phase;
  uint8_t attack; // rate
  uint8_t decay;  //percent of velocity
  uint8_t sustain; //rate
  uint8_t rel;    //rate

};

void voice_update(struct Voice*);

#ifdef __cplusplus
}
#endif
#endif
