#include <avr/io.h>
#include <SPI.h>

void eepromWrite(unsigned short,unsigned char);
unsigned char eepromRead(unsigned char);

unsigned char eepromValue;

void setup() {
  // put your setup code here, to run once:
eepromWrite(0x0A,0x55);
Serial.begin(9600);
eepromValue =  eepromRead(0x0A);
Serial.println(eepromValue);
Serial.println("hello world");
eepromValue =  eepromRead(0x0B);
Serial.println(eepromValue);
}

void loop() {
  // put your main code here, to run repeatedly:

}

unsigned char eepromRead(unsigned char address)
{
  while(EECR & (1<<EEPE));
if (address < 512)
{
  EEAR = address;
}
else
{
  EEAR = 512;
}
  EECR |= (1<<EERE);

  return EEDR;
}

void eepromWrite(unsigned short address,unsigned char data)
{
 while (EECR & (1<<EEPE));

 EECR = (0<<EEPM1) | (0 << EEPM0);
 if (address < 512)
 {
  EEAR = address;
 }
 else 
 {
  EEAR = 511;
 }
 EEDR = data;

 EECR |= (1<<EEMPE);
 EECR |= (1<<EEPE);
}
