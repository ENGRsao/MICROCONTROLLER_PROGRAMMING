#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>  

#define I2C_ADDR    0x3F //0x27  
#define BACKLIGHT_PIN  3
#define En_pin  2//4
#define Rw_pin  1 //5
#define Rs_pin  0 //6
#define D4_pin  4 //0
#define D5_pin  5 //1
#define D6_pin  6 //2
#define D7_pin  7 //3
#define  LED_OFF  0
#define  LED_ON  1

LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
unsigned long start = 0,endtime = 0; 
int i = 0; 
int motorspeed;

void setup() 
{
  lcd.begin (16,2);  // initialize the lcd
  lcd.clear();
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(LED_ON); // Switch on the backlight
  Wire.begin ();  
  attachInterrupt(0,count,FALLING);
  //Serial.begin(9600);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GEAR TACHOMETER");
  lcd.setCursor(0,1);
  lcd.print("MEASURE SPEED");
  delay(1000);
}


void loop() {
  // put your main code here, to run repeatedly:
if (i <= 20)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MOTOR SPEED =");
  lcd.setCursor(0,1);
  lcd.print("MEASURING");
  delay(1000);
}
else 
{
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("MOTOR SPEED =");
   lcd.setCursor(0,1);
   lcd.print(motorspeed);
   lcd.setCursor(6,1);
   lcd.print("RPM");
   delay(1000);
}
}
void count()
{
  i++;
 if(i == 1)
  {
    start = millis();
  }
  
  if(i == 20)
  {
   endtime = millis();
     motorspeed = i * (60000/abs(abs(endtime)-abs(start)));
  }
}
