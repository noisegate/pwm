// playing with low level interrupt and 
// PWM...

#include <avr/interrupt.h>

// vector names
// http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html

int input;

ISR(TIMER2_COMPA_vect) {
  PORTB ^= 1 << 5;//toggle pin 13
  input = analogRead(0);
  //too slow
  //analogWrite(5, input);
  OCR1A = input;
  OCR1B = input;
}

void setup() {
  // put your setup code here, to run once:

  //pin 13 output
  //Data Direction Register port D
  //9, 10, 13 output
  DDRB  |= (1 << 1)| (1<<2) |(1<<5);
  PORTB=0b00000110;
  
  //setup timer 1 for fast PWM
  //clear OC1A on compare match
  //paragraph16 datasheet page 132
  //fastpwm 10 bit and prescaler to 1
  //oca to non-inverting and ocb to inverting
  TCCR1A = ((1<<COM1A1)|(1<<COM1B1)|(1<<COM1B0)|(1<<WGM11)|(1<<WGM10));
  TCCR1B = ((1<<CS10)|(1<<WGM12));
  OCR1A = 10;
  //ICR1H = 0;
  //ICR1L = 255;
  
  //  TCCR0A
  //Timer/Counter Interrupt Mask Register sets 
  //bit Timer/Counter0 Overflow Interrupt Enable
  TIMSK2 = 1<<OCIE2A;//1<<TOIE0;

  //para 15.9 atmel datasheet
  //set to CTC mode, clear on compare match
  TCCR2A = 1<<WGM01;
  TCCR2B |= (1<<CS00);//prescaler 128

  //count till 10
  //timer 2 runs @ 62k so 5KHz is 
  OCR2A = 12;
  
  sei();
}

void loop() {
  // put your main code here, to run repeatedly:

}
