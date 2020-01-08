#include <avr/io.h>
#include <util/delay.h>


void setup() {
  // put your setup code here, to run once:
pinMode(3,OUTPUT);
pinMode(5,OUTPUT);

TCCR2A = _BV(COM2A1)|_BV(COM2B1)| _BV(WGM21)| _BV(WGM20);
TCCR2B = _BV(WGM22)| _BV(CS20);
OCR2A = 63;
OCR2B = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
    if(OCR2B < 63)
      OCR2B += 5;
    else
      OCR2B = 0;
}
