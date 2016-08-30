#include "synth.h"
#include "notes.h"
#include "debug_arduino.h"

#define TUNING 440
#define led PC13
#define OVERCLOCKED 1
#define BUTTON PB12
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
volatile uint32_t currentTick; // will overflow every 24 hours @22 microsecond ticks

void play() {
  //faster to set timer directly than pwmWrite do a timer lookup, then set compare
  //high bits
  uint8_t highbits = playbuffer >> 8;
  uint8_t clearbits = ~highbits;
  pwmTimer.setCompare(TIMER_CH2, highbits); //PB7
  //low bits
  pwmTimer.setCompare(TIMER_CH1, playbuffer &  0x00FF); //PB6

  //writing to 8bit r2r for debug purposes
  GPIOA->regs->BSRR = highbits | (clearbits << 16);

  currentTick++;

  //now we have 22 microseconds @44.1kHz to get next play value
  playbuffer = synth_get_wave(currentTick) + (0xFFFF / 2);
  dwrite_buffer(playbuffer);
}

uint8_t chord[] = {69, 73, 76};
//uint8_t chord[] = {69,73,76,81,85,88,93,97,100};
void chorus(int8_t vol) {
  for (uint8_t i = 0; i < sizeof(chord) / sizeof(chord[0]); i++) {
    synth_note_on(chord[i], vol);
  }
}
void chorus_off() {
  for (uint8_t i = 0; i < sizeof(chord) / sizeof(chord[0]); i++) {
    synth_note_off(chord[i]);
  }
}

void setupPWM() {
  //configure pwm
  pwmTimer.setPrescaleFactor(1);
  pwmTimer.setOverflow(255);
  //(TIMER4->regs).bas->CR1 |= 1<<5; //use center-aligned pwm
  pwmTimer.refresh();
  pinMode(PB6, PWM);
  pinMode(PB7, PWM);
}

void setupPlayTimer() {
  //configure the sample play timer
  sampleTimer.pause();
  sampleTimer.setPrescaleFactor(1);
  sampleTimer.setOverflow(TIMER_PERIOD);
  sampleTimer.setMode(TIMER_CH1, TIMER_OUTPUT_COMPARE);
  sampleTimer.setCompare(TIMER_CH1, 1);
  sampleTimer.attachInterrupt(TIMER_CH1, play);
  sampleTimer.refresh();
  sampleTimer.resume();
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

  setupPWM();
  setupPlayTimer();
  chorus(255);

  //configure input
  pinMode(BUTTON, INPUT_PULLUP);
}

void serialEventRun(void) {
  String inputString = Serial.readString();
  if (inputString != "") {
    commandRecieved(inputString);
  }
}
uint8_t note = 69;
uint8_t vol = 255;
void commandRecieved(String cmd) {
  char chars[6];
  uint8_t tmp;
  switch (cmd.charAt(0)) {
    case 'v':
      cmd.substring(1).toCharArray(chars, sizeof(chars));
      tmp = atoi( chars );
      vol = tmp;
      synth_note_on(note, vol);
      break;
    case 's':
      cmd.substring(1).toCharArray(chars, sizeof(chars));
      tmp = atoi( chars );
      note = tmp;
      synth_note_on(note, vol);
      break;
    case 'e':
      cmd.substring(1).toCharArray(chars, sizeof(chars));
      tmp = atoi( chars );
      synth_note_off(tmp);
      break;
    case 'p':
      dshow(TIMER_PERIOD);
      dshow(sampleTimer.getPrescaleFactor());
      dshow(sampleTimer.getOverflow());
      break;
    case 'd':
      dump_buffer();
      break;
  }
  Serial.println(cmd);
}

int buttonState = HIGH;
void loop() {
  int pushed = digitalRead(BUTTON);
  if (pushed != buttonState) {
    if (pushed == HIGH) {
      chorus_off();
    } else {
      chorus(vol);
    }
    dshow(pushed);
    buttonState = pushed;
  }

  // send data only when you receive data:
  if (Serial.available() > 0) {
    serialEventRun();
  }
}



