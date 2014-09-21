#define clockInterrupt 1
#define latchInterrupt 0
#define dataPin A2

#define dataShift 2
#define dataMask (1 << dataShift)
#define pressLength 4

uint8_t buttonsToPress = B00001000;
uint8_t buttons = buttonsToPress;
boolean pressButton = false;
int counter = 0;

void setup() {
  pinMode(dataPin, OUTPUT);

  attachInterrupt(latchInterrupt, latch, RISING);
  attachInterrupt(clockInterrupt, clock, RISING);

  PORTC |= dataMask;
}

void latch() {
  counter++;
  counter %= pressLength * 2;
  buttons = ~(buttonsToPress * (counter==pressLength));
  PORTC = (PORTC & ~dataMask) | ((buttons & 1) << dataShift);
}

void clock() {
  buttons >>= 1;
  PORTC = (PORTC & ~dataMask) | ((buttons & 1) << dataShift);
}

void loop() {}
