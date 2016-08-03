/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  serial upload instructions for stm32f1 maple mini clone: 
  ftdi rx to PA9
  ftdi tx to PA10 
  ftdi vcc to 3.3v
  ftdi ground to ground
  
  ftdi set to 3.3V
  Boot0 at 1 position (top jumper when usb socket is to your left)

  sketch will run after upload, but not on power on if boot0 is at 1

  press reset if sketch is running to place board back to upload mode
  
 */

#define led PC13


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(led, OUTPUT);

  //set this ground for reduced wiring =)
  pinMode(PC15,OUTPUT);
  digitalWrite(PC15,0);
  
  pinMode(PA0,OUTPUT);
  pinMode(PA1,OUTPUT);
  pinMode(PA2,OUTPUT);
  pinMode(PA3,OUTPUT);
  pinMode(PA4,OUTPUT);
  pinMode(PA5,OUTPUT);
  pinMode(PA6,OUTPUT);
  pinMode(PA7,OUTPUT);
  
  Serial.begin(115200);
}
volatile uint8_t count = 0;

void dacOutput(uint8_t value){
  uint8_t notval = ~value;
  GPIOA->regs->BSRR = value | (notval << 16);
//  digitalWrite(PA0,value & (1<<7));
//  digitalWrite(PA1,value & (1<<6));
//  digitalWrite(PA2,value & (1<<5));
//  digitalWrite(PA3,value & (1<<4));
//  digitalWrite(PA4,value & (1<<3));
//  digitalWrite(PA5,value & (1<<2));
//  digitalWrite(PA6,value & (1<<1));
//  digitalWrite(PA7,value & (1<<0));
}

//outputs 0-255 at 256 x positions
uint8_t taylorCos(uint8_t x) {
  //taylor series approx: 1 - x^2/2!  + x^4/4!
   return 1 - (x*x)/2 ;
}

// the loop function runs over and over again forever
void loop() {
  dacOutput(count);   
  count++;
  delayMicroseconds(10);
}
