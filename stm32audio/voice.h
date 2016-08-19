#ifndef VOICE_H
#define VOICE_H
#ifdef __cplusplus
extern "C" {
#endif

struct Voice {
  //wave characteristics
  int32_t volume;   //should be under 16bits, but using 32bit for intermediate calculations
  int32_t velocity; //should be under 16bits, but using 32bit for intermediate calculations
  uint16_t period; //pitch
  int16_t (*oscillator)(uint16_t vol, uint16_t period, uint32_t phase);

  //adsr envelope
  uint32_t ticks;
  uint8_t adsr_phase;
  
  int8_t attack; // rate
  uint8_t decay;  //percent of velocity
  int8_t sustain; //rate
  int8_t rel;    //rate

};

void voice_update(struct Voice*);

#ifdef __cplusplus
}
#endif
#endif
