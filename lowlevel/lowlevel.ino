// playing with low level interrupt and 
// PWM...

#include <avr/interrupt.h>

#define BIT0 0b00000001
#define BIT1 0b00000010
#define BIT2 0b00000100
#define BIT3 0b00001000
#define BIT4 0b00010000
#define BIT5 0b00100000
#define BIT6 0b01000000
#define BIT7 0b10000000

#define POSPIN 2
#define NEGPIN 7

// vector names
// http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html

unsigned int input;
byte adc;

//ISR(TIMER2_COMPA_vect) {
ISR(ADC_vect){
  PORTB ^= 1 << 5;//toggle pin 13 check samplerate
  //input = analogRead(0);
  //read lowbyte first
  input = ADCL;
  input |=(ADCH<<8);

  adc = (byte)(input>>2);
  
  //input -= 512;

  //So this stuff doesnt workt out with 10bits
  //put twoscomplement flag into PORTC
  //this enables one or the other half
  //of the H-bridge
  
  //PORTD = input & BIT7;

  //using PNP drivers for 1st half and NPN for 
  //2nd half of the bridge
  //and the fact that we use 2's complement
  //the next line should simply do it.
  //ie.: the neg values are inverted so we get
  //inverted pulsewidth, but so is the driver.
  //except for +1 ?? yeah. well crossover distortion
  

  
  
  if (adc<0x80){
    OCR1A = 0x80-adc;
    OCR1B = 0;
  }else{
    OCR1A = 0;
    OCR1B = adc-0x80;
  }
  
  //OCR1A = input>>4;
  //restart ADC conversion
  //dont need thsi in free running mode?ADCSRA |= (1 << ADSC);
  //OCR1B = input;
}

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  //Data Direction Register port D
  //9, 10, 13 output
  DDRB  |= (1 << 1)| (1<<2) |(1<<5);
  PORTB=0b00000110;
  DDRD = 0b10000000;
  DDRC &=(0b1111110);

  //setup ADC
  ADMUX = 0;//use aref and mux = 0
  ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADIE)|(1<<ADPS2);//|(1<<ADPS0);//|(1<<ADPS0);
  ADCSRB = 0;//free running mode// (1 << ADTS2);//timer counter 0 overflow
  DIDR0 = 0b00011111;//digital off;

  //CTC mode
  //TCCR0A = 1 << WGM01;
  //prescaler 
  //TCCR0B |= (1<<CS00);
  //OCR0A=26;
  //setup timer 1 for fast PWM
  //clear OC1A on compare match
  //paragraph16 datasheet page 132
  //fastpwm 10 bit and prescaler to 1
  //oca to non-inverting and ocb to inverting
  //10bit
  //TCCR1A = ((1<<COM1A1)|(1<<COM1B1)|(1<<COM1B0)|(1<<WGM11)|(1<<WGM10));
  //TCCR1B = ((1<<CS10)|(1<<WGM12));
  //fast PWM 8 bits
  //A non-inverted, B nonn-inverted
  TCCR1A = ((1<<COM1A1)|(1<<COM1B1)|(1<<WGM12)|(1<<WGM10));
  TCCR1B = ((1<<CS10));
  OCR1A = 10;
  OCR1B = 10;
  //ICR1 = 0x3FF;
  
  
  //  TCCR0A
  //Timer/Counter Interrupt Mask Register sets 
  //bit Timer/Counter0 Overflow Interrupt Enable
  //TIMSK2 = 1<<OCIE2A;//1<<TOIE0;

  //para 15.9 atmel datasheet
  //set to CTC mode, clear on compare match
  //TCCR2A = 1<<WGM01;
  //TCCR2B |= (1<<CS00);//prescaler 128

  //count till 10
  //timer 2 runs @ 62k so 5KHz is 
  //OCR2A = 6;
  
  sei();
  //start a first conversion... 
  ADCSRA |= (1 << ADSC);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(input);
  //cpu_sleep()
}
