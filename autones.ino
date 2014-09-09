#define clockInterrupt 0
#define latchInterrupt 1

#define dataShift 2
#define dataMask (1 << dataShift)

// Right Left Down Up Start Select B ???
byte buttons;
byte latchCount;

int index=0;
int frame=0;

// what buttons to press
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

