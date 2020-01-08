#include "U8glib.h"
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);  

int VOLTAGE = A1;
int CURRENT = A0;
int voltageValueMIN = 800,voltageValueMAX = 0;
int currentValueMIN = 600,currentValueMAX = 0;
float voltage, current, power, frequency,freq;
long period, powerFactor;
 bool count ;
 int val = 0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(VOLTAGE,INPUT);
pinMode(CURRENT,INPUT);
// flip screen, if required
  // u8g.setRot180();
  

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
attachInterrupt(0,voltageZCD,RISING);
//attachInterrupt(1,currentZCD,RISING);

}

void loop() {
  // put your main code here, to run repeatedly:
   voltage = 0; current = 0;
  for (int j = 0; j < 5; j++)
      {
     for (int i = 0; i <200; i++)
      {
        int current = analogRead(CURRENT);
          if (current < currentValueMIN)
           {
            currentValueMIN =  current;
           }
           if (current > currentValueMAX)
           {
                currentValueMAX = current;
           }
      }
    Serial.print("current value");
    Serial.print(currentValueMIN); Serial.print("-"); Serial.println(currentValueMAX);
       current += (currentValueMAX - currentValueMIN);
       for (int i = 0; i <200; i++)
      {
        int voltage = analogRead(VOLTAGE);
          if (voltage < voltageValueMIN)
           {
            voltageValueMIN =  voltage;
           }
           if (voltage > voltageValueMAX)
           {
              voltageValueMAX = voltage ;
           }
      }
    Serial.print("voltage value");
    Serial.print(voltageValueMIN); Serial.print("-"); Serial.println(voltageValueMAX);
    delay(500);
    voltage += (voltageValueMAX - voltageValueMIN);
    resetParameter();
  }
  if (current > 6)
  {
    current = current / (5 * 24);
  }
  else
  {
    current = 0;
  }
  if (voltage >= 3500)
  {
    voltage = voltage / (5* 3.5);
  }
  else
  {
    voltage = 0;
  }
  power = voltage*current;
      u8g.firstPage();  
     do {
        draw();
      } while( u8g.nextPage() );
}
void resetParameter(){
 voltageValueMIN = 800,voltageValueMAX = 0;
 currentValueMIN = 600,currentValueMAX = 0;
}

void voltageZCD()
{
  count = !count;
  if (count)
    period = millis();
  else
  {
    period = millis() - period;
    
      freq += 1000/period;
      val++;
      if (val == 10)
      {
        frequency = freq/10;
        val = 0; 
        freq = 0;
      }
   
  }
    
}
void currentZCD()
{
 /* if (count)
  {
    Serial.print("powerfactor-");
    Serial.println(millis() - period);
  }*/
}


void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_helvB08);

u8g.drawStr( 20, 20, "POWER MONITOR");
u8g.drawStr( 0, 30, "VOLTAGE:");
u8g.setPrintPos(65, 30);
u8g.print(voltage);
if (voltage < 180){ frequency = 0.0; current = 0.0;}
else {if ((frequency < 47) || (frequency > 65)) frequency = 50;}
u8g.drawStr( 0, 40, "CURRENT:");
u8g.setPrintPos(65, 40);
u8g.print(current);
u8g.drawStr( 0,50, "POWER:");
u8g.setPrintPos(65, 50);
u8g.print(power);
u8g.drawStr( 0, 60, "FREQ:");
u8g.setPrintPos(65, 60);
u8g.print(frequency);
}
