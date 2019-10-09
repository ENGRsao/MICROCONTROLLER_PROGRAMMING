#include <LiquidCrystal.h>
LiquidCrystal lcd(19,18,17,16,15,14); 
int col = 0,row=0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
lcd.begin(20,4);
lcd.clear();
lcd.print("SERIAL-LCD MONITOR");delay(2000);lcd.clear();
lcd.setCursor(0,0);
}

void loop() {
  // put your main code here, to run repeatedly:
if (Serial.available()>0)
{
  char a = Serial.read();
  if ((a == '\r')|| (a == '\n'))
  {
    row++; col = 0;
    if (row == 4)
    {
      row = 0; lcd.clear();
    }
    lcd.setCursor(col,row);
  }
  else
  {
    lcd.write(a);
    col++;
    if ((col == 20)&&(row !=3))
    {
      row++;col=0;
    }
    else if (row == 3)
    {
      row = 0; lcd.clear();
    }
  }
}
}
