#include <stdint.h>
#include "voice.h"


/**
   attack: rate, increase until velocity is hit
   decay: amount, decrease by attach rate until amount is met
   sustain: rate of decrease
   release: rate of decrease, external trigger
*/
void handle_increase(int8_t rate, struct Voice *v) {
  if (rate > 0) {
    v->volume += rate;
  } else if (rate == 0) {
    //no change
  } else if ((v->ticks % rate) == 0) {
    //slow
    v->volume++;
  }
}

void handle_decrease(int8_t rate, struct Voice *v) {
  if (rate > 0) {
    v->volume -= rate;
  } else if (rate == 0) {
    //no change
  } else if ((v->ticks % rate) == 0) {
    //slow
    v->volume--;
  }
}

void voice_update(struct Voice *v) {
  if (v->volume > 0) {//voice is on
    v->ticks++;
    switch (v->adsr_phase) {
      case 1:
        if (v->volume >= v->velocity) {
          v->adsr_phase = 2;
        } else {
          handle_increase(v->attack, v);
        }
        break;
      case 2:
        if (v->volume <= v->velocity * v->decay / 0xFF) {
          v->adsr_phase = 3;
        } else {
          handle_decrease(v->attack, v);
        }
        break;
      case 3: //sustain
        handle_decrease(v->sustain, v);
        break;
      case 4: //release
        handle_decrease(v->rel, v);
        break;
    }

    if (v->volume > UINT16_MAX) {
      v->volume = UINT16_MAX;
    }
    if (v->volume < 0) {
      v->volume = 0;
    }
  }
}
