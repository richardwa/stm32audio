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
  uint8_t highbits = playbuffer >> 8;
  uint8_t clearbits = ~highbits;
  pwmTimer.setCompare(TIMER_CH2, highbits); //PB7
  //low bits
  pwmTimer.setCompare(TIMER_CH1, playbuffer &  0xFF); //PB6

  //writing to 8bit r2r for debug purposes
  GPIOA->regs->BSRR = highbits | (clearbits << 16);
  
  currentTick++;

  //now we have 22 microseconds @44.1kHz to get next play value
  playbuffer = synth_get_wave(currentTick) + (0xFFFF/2);
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

  //set our R2R ladder DAC to output
  pinMode(PA0, OUTPUT);
  pinMode(PA1, OUTPUT);
  pinMode(PA2, OUTPUT);
  pinMode(PA3, OUTPUT);
  pinMode(PA4, OUTPUT);
  pinMode(PA5, OUTPUT);
  pinMode(PA6, OUTPUT);
  pinMode(PA7, OUTPUT);
  pinMode(PC15, OUTPUT); //our ground pin
  digitalWrite(PC15, 0);


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
  
  synth_note_on(69, 255);
//  synth_note_on(73, 255);
//  synth_note_on(76, 255);
//  
//  synth_note_on(81, 255);
//  synth_note_on(85, 255);
//  synth_note_on(88, 255);
//  
//  synth_note_on(93, 255);
//  synth_note_on(97, 255);
//  synth_note_on(100, 255);

}

void serialEventRun(void) {
  String inputString = Serial.readString();
  if (inputString != "") {
    commandRecieved(inputString);
  }
}
void commandRecieved(String cmd) {
  char chars[6];
  uint8_t tmp;
  switch (cmd.charAt(0)) {
    case 's':
      cmd.substring(1).toCharArray(chars, sizeof(chars));
      tmp = atoi( chars );
      synth_note_on(tmp, 255);
      break;
    case 'e':
      cmd.substring(1).toCharArray(chars, sizeof(chars));
      tmp = atoi( chars );
      synth_note_off(tmp);
      break;
    case 'p':
      Serial.println(TIMER_PERIOD);
      Serial.println(sampleTimer.getPrescaleFactor());
      Serial.println(sampleTimer.getOverflow());
      break;
    case 'd':
      debugWrite = false;
      for (tmp = 0;tmp < DEBUG_SIZE; tmp++){
        Serial.print(debugbuffer[tmp],HEX);
        Serial.print(' ');
      }
      Serial.println();
      debugWrite = true;
      break;
  }
  Serial.println(cmd);
}


void loop() {
  if (currentTick % 32 == 0) {
    synth_env_update();
  }
  
  // send data only when you receive data:
  if (Serial.available() > 0) {
    serialEventRun();
  }
}


