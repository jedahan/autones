#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

#define dataShift 2
#define dataMask (1 << dataShift)

// Off, A, B, sElect, Start, Up, Down, Left, Right
#define O (0 << 0)
#define A (1 << 0)
#define B (1 << 1)
#define E (1 << 2)
#define S (1 << 3)
#define U (1 << 4)
#define D (1 << 5)
#define L (1 << 6)
#define R (1 << 7)

#define HZ 60

// a movie is how many frames * what buttons to press
uint8_t movie[][2] =  { {4, S}, {HZ, A} };
uint8_t movie_size = ARRAY_SIZE(movie);
uint8_t index = 0;

uint8_t framesLeft = movie[0][0];
uint8_t buttons = ~(movie[0][1]);

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
  PORTC = (PORTC & ~dataMask) | ((buttons & 1) << dataShift);
  if(framesLeft == 0 && index <= movie_size){
    index++;
    framesLeft = movie[index][0];
  }
  buttons = ~(movie[index][1]);
  framesLeft--;
}

// clock
ISR(INT1_vect) {
  buttons >>= 1;
  PORTC = (PORTC & ~dataMask) | ((buttons & 1) << dataShift);
}
