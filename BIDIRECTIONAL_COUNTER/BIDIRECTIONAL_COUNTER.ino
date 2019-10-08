#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>  // F Malpartida's NewLiquidCrystal library
#define I2C_ADDR    0x3F  // Define I2C Address for controller
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
LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

int bulb = 10;
int trigpin = 2;
int echopin = 3;
int member;
int x;
void setup() {
  // put your setup code here, to run once:4
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(LED_ON);
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  pinMode(bulb, OUTPUT);
  lcd.setCursor(0, 0);
  lcd.print("VISITOR COUNTER");
  lcd.setCursor(0, 1);
  lcd.print("SYSTEM");
  delay(2000);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("PLEASE SET");
  lcd.setCursor(2, 1);
  lcd.print("TRANSMITTER");
  delay(5000);
  member = 0;
}


void loop() {
  // put your main code here, to run repeatedly:
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("NO OF VISITOR");
  lcd.setCursor(0, 1);
  lcd.print(member);
  delay(900);
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  int echoval = pulseIn(echopin, HIGH);
  int distance = echoval / 60;
  if  (distance <= 5)
  {
    delay(100);
    int a = analogRead(A0);
    int b = analogRead(A1);
    Serial.print(a); Serial.print("    "); Serial.print(b); Serial.print("    "); Serial.println(member);
    if ((a < 50) && (b > 50))
    {
      member++;
    }
    if ((b < 50) && (a > 50))
    {
      if (member == 0)
      {
        member = 0;
      }
      else
      {
        member--;
      }
    }
    if ((a < 50) && (b < 50))
    {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("TRANSMITTER");
      lcd.setCursor(2, 1);
      lcd.print("NOT SET");
    }
  }

  if (member > 0)
  {
    digitalWrite(bulb, HIGH);
  }
  else
  {
    digitalWrite(bulb, LOW);
  }

}
