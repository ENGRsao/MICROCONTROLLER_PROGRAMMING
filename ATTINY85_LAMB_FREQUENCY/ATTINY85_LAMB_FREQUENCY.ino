#include <avr/io.h>
#define F_CPU 8000000UL
void setup() {
  // put your setup code here, to run once:
  DDRB = (1<<DDB0) | (1<<DDB1) ;
  TCCR1 = (1<<CS10) | (1<<PWM1A) |(1<<COM1A1);
  PLLCSR = (1<<PCKE) | (1 << PLLE);
  OCR1C = 255;
  OCR1A = 127;
  //OCR1B = 254;    
}

void loop() {
  // put your main code here, to run repeatedly:

}
