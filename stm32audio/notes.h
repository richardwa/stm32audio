#ifndef NOTES_H
#define NOTES_H
// frequency information in terms of period 
extern uint16_t note_periods[128];
void notes_init(uint32_t sampleRate, uint32_t tuning);
#endif
