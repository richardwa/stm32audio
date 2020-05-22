#ifndef NOTES_H
#define NOTES_H
#ifdef __cplusplus
extern "C" {
#endif

// frequency information in terms of period 
extern uint16_t note_periods[128];
void notes_init(uint32_t sampleRate, uint32_t tuning);

#ifdef __cplusplus
}
#endif
#endif
