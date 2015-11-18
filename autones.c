#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t button_number;
// we are using naive run-length encoding
// frames[0] corresponds to how many frames we should be pressing buttons[0]
PROGMEM const uint16_t frames[] = {30,1,30,1,30,10,30,10,30,10,30,10,30};
// buttons are A B s S U D L R
#define A_ 1 << 7
#define B_ 1 << 6
#define s_ 1 << 5
#define S_ 1 << 4
#define U_ 1 << 3
#define D_ 1 << 2
#define L_ 1 << 1
#define R_ 1 << 0

PROGMEM const uint8_t buttons[] =  {0,S_,0,S_,0,U_,0,R_,0,D_,0,L_,0};

uint16_t movie_size = sizeof(buttons);
uint16_t index = 0;

uint8_t slowdown = 60;
uint16_t frame = slowdown * frames[0];
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
  // enable timer 0
  TIMSK0 |= (1 << TOIE0);
  // set timer 0 mode to CTC
  TCCR0A |= (1 << WGM01);
  // count to 0xF9 (249)
  OCR0A = 0xF9;
  // set the ISR COMPA vect
  TIMSK0 |= 1 << OCIE0A;
  // global interrupt enable
  sei();

  while(1){
    if(index >= movie_size) {
      index = 0;
      PORTB ^= _BV(PB0); // toggle LED
    }
  };
  return 1;
}

// every time we see a latch, reset the current buttons
ISR(INT0_vect) {
  // reset to the current button
  button = ~(pgm_read_byte_near(buttons + index));
  TCCR0B &= ~(1 << CS02); // disable this timer
  TCCR0B |= (1 << CS02); // set prescaler to 256, to start the 4ms timer
  PORTB ^= _BV(PB0); // toggle LED
}

// input timeout for timer0
// once we have heard the last latch in a while, advance the frame
ISR (TIMER0_OVF_vect) {
  TCCR0B &= ~(1 << CS02); // disable this timer
  if(frame == 0) { // run-length decoding, to advance the index
    index++;
    frame = slowdown * pgm_read_word_near(frames + index);
  }
  PORTB = button;
  button >>= 1;
  frame--;
}

// on clock, shift out each button
ISR(INT1_vect) {
  _delay_ms(1);
  PORTB = button;
  button >>= 1;
}
