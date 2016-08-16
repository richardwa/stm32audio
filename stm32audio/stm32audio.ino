#include "synth.h"
#include "notes.h"

#define TUNING 440
#define led PC13
#define OVERCLOCKED 1
//overclocked to 128 mhz

#define SAMPLE_RATE 31250
#if OVERCLOCKED == 1
#define TIMER_PERIOD 128000000 / SAMPLE_RATE
#define BAUD 64800
#else
#define TIMER_PERIOD  72000000 / SAMPLE_RATE
#define BAUD 115200
#endif

HardwareTimer sampleTimer(1);
HardwareTimer pwmTimer(4);

uint16_t playbuffer; //next value to play
#define DEBUG_SIZE 200
boolean debugWrite = true;
uint16_t debugbuffer[DEBUG_SIZE];

volatile uint32_t currentTick; // will overflow every 24 hours @22 microsecond ticks

void play() {
  //faster to set timer directly than pwmWrite do a timer lookup, then set compare
  //high bits
  pwmTimer.setCompare(TIMER_CH2, playbuffer >> 8); //PB7
  //low bits
  pwmTimer.setCompare(TIMER_CH1, playbuffer &  0xFF); //PB6
  currentTick++;

  //now we have 22 microseconds @44.1kHz to get next play value
  playbuffer = synth_get_wave(currentTick) + INT16_MAX;
  if (debugWrite){
    debugbuffer[currentTick % DEBUG_SIZE] = playbuffer;
  }
}



void setup() {
  //systick_disable();
  Serial.begin(BAUD);
  delay(100);
  Serial.println("starting");
  notes_init(SAMPLE_RATE, TUNING);

  // initialize digital pin 13 as an output.
  pinMode(led, OUTPUT);


  //configure pwm
  pwmTimer.setPrescaleFactor(1);
  pwmTimer.setOverflow(255);
  pwmTimer.refresh();
  pinMode(PB6, PWM);
  pinMode(PB7, PWM);

  //configure the sample play timer
  sampleTimer.pause();
  sampleTimer.setPrescaleFactor(1);
  sampleTimer.setOverflow(TIMER_PERIOD);
  sampleTimer.setMode(TIMER_CH1, TIMER_OUTPUT_COMPARE);
  sampleTimer.setCompare(TIMER_CH1, 1);
  sampleTimer.attachInterrupt(TIMER_CH1, play);
  sampleTimer.refresh();
  sampleTimer.resume();
  
  //synth_note_on(69, 255);
  //synth_note_on(76, 255);

}

void serialEventRun(void) {
  String inputString = Serial.readString();
  if (inputString != "") {
    commandRecieved(inputString);
  }
}
void commandRecieved(String cmd) {
  char chars[6];
  uint8_t cmdVal;
  switch (cmd.charAt(0)) {
    case 's':
      cmd.substring(1).toCharArray(chars, sizeof(chars));
      cmdVal = atoi( chars );
      synth_note_on(cmdVal, 255);
      break;
    case 'e':
      cmd.substring(1).toCharArray(chars, sizeof(chars));
      cmdVal = atoi( chars );
      synth_note_off(cmdVal);
      break;
    case 'd':
      Serial.println(TIMER_PERIOD);
      Serial.println(sampleTimer.getPrescaleFactor());
      Serial.println(sampleTimer.getOverflow());
      break;
  }
  Serial.println(cmd);
}

void testVolume(uint32_t i){
  uint8_t tmp = i % 256;
  uint16_t low = UINT16_MAX;
  uint16_t high = 0;
  synth_note_on(69, tmp);
  Serial.print(tmp);
  Serial.print(' ');
  debugWrite = false;
  for (tmp = 0; tmp<DEBUG_SIZE;tmp++){
    if (debugbuffer[tmp] < low){
      low = debugbuffer[tmp];
    }
    if (debugbuffer[tmp] > high){
      high = debugbuffer[tmp];
    }
  }
  Serial.print(high - low);
  Serial.println();
  debugWrite = true;
}

void loop() {
  if (currentTick % 100 == 0) {
    synth_env_update();
  }
  
  if (currentTick % 50000 == 0) {
    testVolume(currentTick / 50000);
  }
  
  // send data only when you receive data:
  if (Serial.available() > 0) {
    serialEventRun();
  }
}


