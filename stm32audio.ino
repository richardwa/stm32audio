
#define led PC13

//playing at 22 micros period, we need the buffer to cover at least 0.05 seconds for the low notes
//highest pitch alternating +/- which comes to 20khz
//lowest pitch is all one period in 0.05
#define bufferLength 2000
#define bufferHeight 256
#define sampleRate 22000
int8_t playbuffer[bufferLength] = {0}; //contains negatives
volatile uint16_t bufferPos = 0;
void play() {
  uint8_t value = playbuffer[bufferPos] + bufferHeight / 2; //convert to unsigned
  uint8_t notval = ~value;
  GPIOA->regs->BSRR = value | (notval << 16);
  bufferPos = (bufferPos + 1) % bufferLength;
}

void printBuffer() {
  for (uint16_t i = 0; i < bufferLength; i++) {
    Serial.print(playbuffer[i]+bufferHeight / 2,HEX);
    Serial.print(" ");
  }
  Serial.println();
}

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  printBuffer();
  sawtooth(440);
  sawtooth(660);
  printBuffer();

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

  //configure the sample timer
  HardwareTimer sampleTimer(1);
  sampleTimer.pause();

  sampleTimer.setPeriod(1000000 / sampleRate);
  sampleTimer.setMode(TIMER_CH1, TIMER_OUTPUT_COMPARE);
  sampleTimer.setCompare(TIMER_CH1, 1);
  sampleTimer.attachInterrupt(TIMER_CH1, play);

  sampleTimer.refresh();
  sampleTimer.resume();

}


//set a sawtooth wave into buffer;
void sawtooth(uint16_t freq) {
  uint16_t period = sampleRate / freq;
  for (uint16_t i = 0; i < bufferLength; i++) {
    playbuffer[i] += ((i % period) * bufferHeight / period - bufferHeight / 2)/2;
  }
}

//taylor series cos approx: 1 - x^2/2!  + x^4/4!
#define inputLength 10
char inputBuffer[inputLength];
uint8_t inputIndex = 0;
boolean inputDone = false;
void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    char in = Serial.read();
    inputBuffer[inputIndex] = in;
    if (in == '\n' || inputIndex == inputLength - 1) {
      inputDone = true;
    } else {
      inputIndex++;
    }
  }
  
  if (inputDone) {
    Serial.write(inputBuffer, inputIndex);
    Serial.println();
    uint16_t value = atol(inputBuffer);
    sawtooth(value);
    printBuffer();
    inputIndex = 0;
    inputDone = false;
  }
}


