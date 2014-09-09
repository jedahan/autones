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
#define clockInterrupt 0
#define latchInterrupt 1

#define dataShift 2
#define dataMask (1 << dataShift)

// Right Left Down Up Start Select B ???
byte buttons;
byte latchCount;

int index=0;
int frame=0;

// what buttons to press, 
byte button[] = { 8, 0, 1};
byte frames[] = { 1*15, 6*15, 0};

uint16_t nextframe=frames[0];

void setup() {
  pinMode(A2, OUTPUT);

  latchCount = 0;

  attachInterrupt(latchInterrupt, latch, RISING);
  attachInterrupt(clockInterrupt, clock, RISING);

  PORTC |= dataMask;
}

void latch() {
  if (latchCount % 4 == 0)
  {
    latchCount = 0;
    buttons = button[index];
    // Keep all values except 3 | set the third value 
    PORTC = (PORTC & ~dataMask) | ((~(buttons & 1)) << dataShift);
    buttons >>= 1;
    frame++;
    if(frame > nextframe && index < sizeof(frames)-1 ){
      index++;
      nextframe += frames[index];
    }
  }
  latchCount++;
}

void clock() {  
    // Keep all values except 3 | set the third value 
    PORTC = (PORTC & ~dataMask) | ((~(buttons & 1)) << dataShift);
    buttons >>= 1;
}

void loop() {
  /* wait */
}

