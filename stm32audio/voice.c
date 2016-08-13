#include <stdint.h>
#include "voice.h"

/**
   attack: rate, increase until velocity is hit
   decay: amount, decrease by attach rate until amount is met
   sustain: rate of decrease
   release: rate of decrease, external trigger
*/
void voice_update(struct Voice *v) {
  if (v->volume > 0) {
    v->ticks++;
    switch (v->adsr_phase) {
      case 1:
        if (v->volume >= v->velocity) {
          v->adsr_phase = 2;
        } else if (v->attack == 0) {
          v->volume += 2;
        } else if (v->ticks % v->attack == 0) {
          v->volume++;
        }
        break;
      case 2:
        if (v->volume <= v->velocity - v->velocity * v->decay / 0xFF ) {
          v->adsr_phase = 3;
        } else if (v->attack == 0) {
          v->volume -= 2;
        } else if (v->ticks % v->attack == 0) {
          v->volume--;
        }
        break;
      case 3: //sustain
        if (v->sustain < 0xFF && v->ticks % (v->sustain * v->period / 100 ) == 0) {
          v->volume--;
        }
        break;
      case 4: //release
        if (v->ticks % v->rel == 0) {
          v->volume--;
        }
        break;
    }
  }
}
