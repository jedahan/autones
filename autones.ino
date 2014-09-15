/*
 *  interrupt 0 and 1 are tied to digital pins 2 and 3, respectively
 *  they return in ~23ns, which is plenty of time, since the NES
 *  reads the controller at 60hz * 8 buttons = 480hz ~= 2ms.
 
 *  When testing with MagMax, I found trying to set the buttons on
 *  every frame (latchCount % 1 == 0) wouldn't work. I think its
 *  because the game programming may do some controller re-reading...

 * NOTES:
 *  the NES pulses the latch pin @60hz, and reads A on the falling edge
 *  then, the NES pulses the clock pin 8 times, reading the rest of the buttons on the falling edge
 *    A B Select Start Up Down Left Right (we reverse this order when doing the bit shifting logic)
 *  the controller pulls the data pin LOW to asset that a button is pressed (hence the inversion)
 *  thats about it!

*/

#define OFF 0
#define A 1 << 0
#define B 1 << 1
#define SELECT 1 << 2
#define START 1 << 3
#define UP 1 << 4
#define DOWN 1 << 5
#define LEFT 1 << 6
#define RIGHT 1 << 7

#define HZ 60

// This is our program!
byte button[] = { START,  OFF, A};
byte frames[] = {  1*HZ, 6*HZ, 0};

#define clockInterrupt 0
#define latchInterrupt 1

#define dataShift 2
#define dataMask (1 << dataShift)

byte buttons = 0;
uint16_t frame = 0;
int index=0;

uint16_t nextframe=frames[0];

void setup() {
  pinMode(A2, OUTPUT);

  attachInterrupt(latchInterrupt, latch, RISING);
  attachInterrupt(clockInterrupt, clock, RISING);

  PORTC |= dataMask;
}

void latch() {
  // Low is on!
  buttons = ~button[index];
  // Keep all values except 3 | set the third value 
  PORTC = (PORTC & ~dataMask) | ((buttons & 1) << dataShift);
  buttons >>= 1;
  frame++;
  if(frame > nextframe && index < sizeof(frames)-1 ){
    nextframe += frames[++index];
  }
}

void clock() {  
  // Keep all values except 3 | set the third value 
  // Equivalent to digitalWrite(dataPin, (buttons & 1))
  PORTC = (PORTC & ~dataMask) | ((buttons & 1) << dataShift);
  buttons >>= 1;
}

void loop() {
  /* wait */
}

