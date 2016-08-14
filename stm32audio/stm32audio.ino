#include "synth.h"
#include "notes.h"

#define TUNING 440
#define led PC13

#define OVERCLOCKED 1
//overclocked to 128 mhz

#if OVERCLOCKED == 1
#define SAMPLE_RATE 36000
#define TIMER_PERIOD 1800000 / SAMPLE_RATE
#define BAUD 64800
#else
#define SAMPLE_RATE 31250
#define TIMER_PERIOD 1000000 / SAMPLE_RATE
#define BAUD 115200
#endif


uint8_t playbuffer; //next value to play
volatile uint32_t currentTick; // will overflow every 24 hours @22 microsecond ticks

void play() {
  //send buffer to pins without variable delay
  uint8_t clearbits = ~playbuffer;
  GPIOA->regs->BSRR = playbuffer | (clearbits << 16);
  currentTick++;

  //now we have 22 microseconds @44.1kHz to get next play value
  playbuffer = synth_get_wave(currentTick) * 128 / 1000 + 128;
}


HardwareTimer sampleTimer(1);
void setup() {
  Serial.begin(BAUD);
  notes_init(SAMPLE_RATE, TUNING);
  synth_note_on(69, 255);
  synth_note_on(76, 255);

  // initialize digital pin 13 as an output.
  pinMode(led, OUTPUT);
  //set this ground for reduced wiring =)
  pinMode(PC15, OUTPUT);
  digitalWrite(PC15, 0);

  //set our R2R ladder DAC to output
  pinMode(PA0, OUTPUT);
  pinMode(PA1, OUTPUT);
  pinMode(PA2, OUTPUT);
  pinMode(PA3, OUTPUT);
  pinMode(PA4, OUTPUT);
  pinMode(PA5, OUTPUT);
  pinMode(PA6, OUTPUT);
  pinMode(PA7, OUTPUT);

  //configure the sample play timer
  sampleTimer.pause();
  sampleTimer.setPeriod(TIMER_PERIOD);
  sampleTimer.setMode(TIMER_CH1, TIMER_OUTPUT_COMPARE);
  sampleTimer.setCompare(TIMER_CH1, 1);
  sampleTimer.attachInterrupt(TIMER_CH1, play);

  sampleTimer.refresh();
  sampleTimer.resume();

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
      Serial.println(sampleTimer.getPrescaleFactor());
      Serial.println(sampleTimer.getOverflow());
      break;
  }
  Serial.println(cmd);
}
void loop() {
  if (currentTick % 100 == 0) {
    synth_env_update();
  }
  // send data only when you receive data:
  if (Serial.available() > 0) {
    serialEventRun();
  }
}


