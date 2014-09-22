#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define dataShift 2
#define dataMask (1 << dataShift)
#define pressLength 4

uint8_t buttonsToPress = 0b00001000;
uint8_t buttons = buttonsToPress;
uint8_t counter = 0;

int main(void) {
  DDRC |= 0b00000100; // pinMode(A2, OUTPUT)
  EICRA = 0b00001111; // INT0,INT1 RISING
  EIMSK = 0b00000011; // Enable INT0, INT1
  PORTC |= dataMask;  // digitalWrite(A2, HIGH)
  sei(); // enable interrupts

  while(1){ };
  return 1;
}

// latch
ISR(INT0_vect) {
  // every <pressLength> frames, switch between the button / OFF
  counter++;
  counter %= pressLength * 2;
  buttons = ~(buttonsToPress * (counter==pressLength));
  PORTC = (PORTC & ~dataMask) | ((buttons & 1) << dataShift);
}

ISR(INT1_vect) {
  buttons >>= 1;
  PORTC = (PORTC & ~dataMask) | ((buttons & 1) << dataShift);
}
