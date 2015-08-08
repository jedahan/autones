#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/io.h>

uint8_t button_number;
// we are using naive run-length encoding
// frames[0] corresponds to how many frames we should be pressing buttons[0]
PROGMEM const uint16_t frames[] = {10,10,10,10};
// buttons are A B s S U D L R
#define A_ 1 << 7
#define B_ 1 << 6
#define s_ 1 << 5
#define S_ 1 << 4
#define U_ 1 << 3
#define D_ 1 << 2
#define L_ 1 << 1
#define R_ 1 << 0

PROGMEM const uint8_t buttons[] =  {U_,R_,D_,L_};

uint16_t movie_size = sizeof(buttons);
uint16_t index = 0;

uint16_t frame = frames[0];
uint8_t button = ~(buttons[0]);

int main(void) {
  // set pins 8-13 as outputs
  DDRB = 0b11111111;
  // fire interrupt 0 on a rising edge (of pin 2, latch)
  EICRA = (1<<ISC01) | (1<<ISC00);
  // fire interrupt 1 on a rising edge (of pin 3, clock)
  EICRA = (1<<ISC11) | (1<<ISC10);
  // enable interrupt 1 and interrupt 0
  EIMSK = (1<<INT1) | (1<<INT0);
  // global interrupt enable
  sei();

  while(1){
    if(index == movie_size) {
      cli();
    }
  };
  return 1;
}

// latch
ISR(INT0_vect) {
  if(frame == 0){
    index++;
    frame = pgm_read_word_near(frames + index);
    button = ~(pgm_read_byte_near(buttons + index));
  }
  PORTB = button;
  button >>= 1;
  frame--;
}

// clock
ISR(INT1_vect) {
  _delay_ms(1);
  PORTB = button;
  button >>= 1;
}
