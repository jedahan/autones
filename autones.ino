#define clockInterrupt 0
#define latchInterrupt 1

#define dataShift 2
#define dataMask (1 << dataShift)

int frame = 0;
int buttons = ~(8);

void setup() {
  pinMode(A2, OUTPUT);

  attachInterrupt(latchInterrupt, latch, RISING);
  attachInterrupt(clockInterrupt, clock, RISING);

  PORTC |= dataMask;
}

void latch() {
  if(frame < 1){
    frame++;
  }
  if(frame >= 1){
    buttons = ~(1);
  }
  PORTC = (PORTC & ~dataMask) | ((buttons & 1) << dataShift);
  buttons >>= 1;
}

void clock() {  
  PORTC = (PORTC & ~dataMask) | ((buttons & 1) << dataShift);
  buttons >>= 1;
}

void loop() {
}

