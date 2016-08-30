#ifndef DEBUG_ARDUINO_H 
#define DEBUG_ARDUINO_H
#ifdef __cplusplus
extern "C" {
#endif
#define DEBUG 1

#if DEBUG == 1
#include <stdint.h>
#include <Arduino.h>
#include <stdbool.h>

void dwrite_buffer(uint16_t val);
void dump_buffer(void);
#define dshow(expression) Serial.print("# "); Serial.print( #expression ); Serial.print( ": " ); Serial.println( expression )
#define dprint(expression) Serial.println( expression )


#define DEBUG_SIZE 200
uint16_t debugbuffer[DEBUG_SIZE];
uint16_t pos;
bool canWrite = true;
void dwrite_buffer(uint16_t val) {
  if (canWrite) {
    debugbuffer[pos] = val;
    pos = (pos + 1) % DEBUG_SIZE;
  }
}
void dump_buffer() {
  uint16_t i;
  canWrite = false;
  for (i = 0; i < DEBUG_SIZE; i++) {
    Serial.print(debugbuffer[i]);
    Serial.print('\t');
  }
  Serial.println();
  canWrite = true;
}


#else
//debug off
#define dprintf(...)
#define dshow(expression) 
#define dwrite_buffer(...)
#define dump_buffer(...)
#endif



#ifdef __cplusplus
}
#endif
#endif
