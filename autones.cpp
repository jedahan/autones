#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

#define dataShift 2
#define dataMask (1 << dataShift)

#define debugPortDMask 0b11110000
#define debugPortBMask 0b00001111

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

uint8_t button_number;

struct frame {
  uint16_t length;
  uint8_t buttons;
};

// a movie is how many frames * what buttons to press
struct frame movie[] =  {{71,0},{2,8},{672,0},{11,16},{10,0},{21,32},{10,0},{14,16},{49,0},{31,128},{2,192},{31,64},{1,192},{21,128},{5,0},{8,64},{16,0},{17,64},{5,80},{12,16},{38,128},{4,160},{32,32},{13,96},{38,64},{5,192},{32,128},{13,160},{14,128},{7,144},{12,16},{13,80},{1,64},{2,66},{6,2},{4,0},{5,2},{4,0},{5,2},{8,0},{18,16},{23,0},{24,128},{3,144},{33,16},{10,0},{17,64},{2,0},{25,32},{14,0},{10,16},{23,0},{13,16},{5,0},{7,2},{5,0},{5,2},{6,0},{5,2},{14,0},{5,2},{8,0},{47,128},{2,192},{11,64},{9,96},{4,64},{18,0},{10,32},{45,96},{7,32},{6,96},{15,32},{10,160},{49,128},{7,0},{7,2},{7,0},{5,2},{8,0},{4,2},{7,0},{4,2},{1,66},{7,64},{4,66},{7,64},{2,66},{3,2},{8,0},{4,32},{5,34},{1,32},{5,0},{7,2},{6,0},{5,2},{5,0},{5,2},{6,0},{5,2},{4,0},{1,64},{5,66},{6,64},{2,66},{4,2},{3,0},{3,64},{6,66},{7,0},{5,2},{6,0},{7,2},{5,0},{5,66},{6,64},{7,2},{5,0},{6,2},{4,0},{3,64},{7,66},{4,64},{3,0},{6,2},{4,0},{10,2},{9,0},{22,16},{10,0},{38,2},{19,0},{11,16},{8,0},{11,128},{2,144},{7,16},{27,144},{21,0},{4,32},{10,0},{10,64},{4,2},{5,18},{27,80},{13,64},{4,0},{10,32},{8,160},{32,128},{15,144},{14,128},{11,160},{7,32},{4,96},{27,64},{676,0}};

uint16_t movie_size = ARRAY_SIZE(movie);
uint16_t index = 0;

uint16_t framesLeft = movie[0].length;
uint8_t buttons = ~(movie[0].buttons);

int main(void) {
  DDRB |= debugPortBMask;
  DDRD |= debugPortDMask;
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
  button_number = 0;
  if(framesLeft == 0 && index < movie_size - 1){
    index++;
    framesLeft = movie[index].length;
  }
  buttons = ~(movie[index].buttons);
  PORTD = (PORTD & ~debugPortDMask) | (~buttons & debugPortDMask);
  PORTB = (PORTB & ~debugPortBMask) | (~buttons & debugPortBMask);
  PORTC = (PORTC & ~dataMask) | ((buttons & 1) << dataShift);
  framesLeft--;
}

// clock
ISR(INT1_vect) {
  button_number++;
  buttons >>= 1;
  if(button_number < 8){
    PORTC = (PORTC & ~dataMask) | ((buttons & 1) << dataShift);	
  } else {
    PORTC = (PORTC & ~dataMask) | (1 << dataShift);	
  }

}
