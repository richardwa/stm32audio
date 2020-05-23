#include <Arduino.h>
#include "synth.h"
#include "notes.h"
#include "debug_arduino.h"

#define TUNING 440
#define led PC13
#define OVERCLOCKED 0
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

HardwareTimer sampleTimer(TIM1);
HardwareTimer pwmTimer(TIM4);

uint16_t playbuffer; //next value to play
volatile uint32_t currentTick; // will overflow every 24 hours @22 microsecond ticks

void play() {
  //faster to set timer directly than pwmWrite do a timer lookup, then set compare
  //high bits
  uint8_t highbits = playbuffer >> 8;
  pwmTimer.setCaptureCompare(2, highbits); //PB7
  //low bits
  pwmTimer.setCaptureCompare(1, playbuffer &  0x00FF); //PB6

  currentTick++;

  //now we have 22 microseconds @44.1kHz to get next play value
  playbuffer = synth_get_wave(currentTick) + (0xFFFF / 2);
  dwrite_buffer(playbuffer);
}

uint8_t chord[] = {69, 73, 76};
//uint8_t chord[] = {69,73,76,81,85,88,93,97,100};
void chorus(int8_t vol) {
  for (uint8_t i = 0; i < sizeof(chord) / sizeof(chord[0]); i++) {
    synth_note_on(0, chord[i], vol);
  }
}
void chorus_off() {
  for (uint8_t i = 0; i < sizeof(chord) / sizeof(chord[0]); i++) {
    synth_note_off(0, chord[i]);
  }
}

void setupPWM() {
  pwmTimer.setPrescaleFactor(1);
  pwmTimer.setOverflow(255);
  //(TIMER4->regs).bas->CR1 |= 1<<5; //use center-aligned pwm
  pwmTimer.refresh();
  pinMode(PB6, TIMER_OUTPUT_COMPARE_PWM1);
  pinMode(PB7, TIMER_OUTPUT_COMPARE_PWM1);
}

void setupPlayTimer() {
  //configure the sample play timer
  sampleTimer.pause();
  sampleTimer.setPrescaleFactor(1);
  sampleTimer.setOverflow(TIMER_PERIOD);
  sampleTimer.setMode(1, TIMER_OUTPUT_COMPARE);
  sampleTimer.setCaptureCompare(1, 1);
  sampleTimer.attachInterrupt(1, (void (*)(HardwareTimer *)) play);
  sampleTimer.refresh();
  sampleTimer.resume();
}

void setup() {
  notes_init(SAMPLE_RATE, TUNING);
  
  //systick_disable();
  Serial.begin(BAUD);
  Serial.println("starting");
  Serial1.begin(31250); //midi listener

  // initialize digital pin 13 as an output.
  pinMode(led, OUTPUT);

  setupPWM();
  setupPlayTimer();
  chorus(255);

  //configure input
  pinMode(BUTTON, INPUT_PULLUP);
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
      synth_note_on(0, note, vol);
      break;
    case 's':
      cmd.substring(1).toCharArray(chars, sizeof(chars));
      tmp = atoi( chars );
      note = tmp;
      synth_note_on(0, note, vol);
      break;
    case 'e':
      cmd.substring(1).toCharArray(chars, sizeof(chars));
      tmp = atoi( chars );
      synth_note_off(0, tmp);
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

#define MIDI_MESSAGE_SIZE 3
uint8_t midiMessageIndex;
uint8_t midiMessage[3];


//midi messages are 3 bytes
void handleMidi() {
  uint8_t cmd = midiMessage[0] & 0xf0;
  uint8_t channel = midiMessage[0] & 0x0f;
  uint8_t note = midiMessage[1];
  uint8_t velocity = midiMessage[2];

  switch (cmd) {
    case 0x90:
      synth_note_on(channel, note, velocity * 2);
      break;
    case 0x80:
      synth_note_off(channel, note);
      break;
    case 0xb0:
      //
      break;

  }

  Serial.print("# ");
  Serial.print(midiMessage[0], HEX);
  Serial.print(" ");
  Serial.print(midiMessage[1], HEX);
  Serial.print(" ");
  Serial.print(midiMessage[2], HEX);
  Serial.println();
}

void loop()
{
  // send data only when you receive data:
  if (Serial.available() > 0) {
    String inputString = Serial.readString();
    if (inputString != "") {
      dshow(inputString);
      commandRecieved(inputString);
    }
  }

  while (Serial1.available())
  {
    uint8_t a = Serial1.read();
    Serial.println(a, HEX);
    //only first byte can be greater than 127
    if (a > 127) {
      midiMessageIndex = 0;
    }

    midiMessage[midiMessageIndex] = a;
    midiMessageIndex++;

    if (midiMessageIndex >= MIDI_MESSAGE_SIZE) {
      handleMidi();
      midiMessageIndex = 0;
    }
  }
}




