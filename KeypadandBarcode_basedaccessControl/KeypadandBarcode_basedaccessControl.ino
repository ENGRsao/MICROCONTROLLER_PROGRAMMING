#include <Keypad.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h> 
#include <Servo.h> 

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
LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char password[5] = {'2','0','1','0','0'} ;
char input[5];
char keys[ROWS][COLS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};
byte rowPins[ROWS] = {9,8,7,6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5,4,3,2}; //connect to the column pinouts of the keypad
Keypad mykeypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
int buzzer = 13;
Servo servomotor; 
int i =0,value = 0;
bool verify;
bool idval ;
int id;
unsigned long keeptime,start,endtime;
void setup()
{
  Serial.begin(9600);
  pinMode(buzzer,OUTPUT);
  servomotor.attach(10);
  lcd.begin (16,2);  // initialize the lcd
   lcd.clear();
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
 lcd.setBacklight(LED_ON);
 Wire.begin();
  digitalWrite(buzzer,LOW);
  lcd.setCursor(0,0);
  lcd.print("WELCOME TO ACCESS");
  lcd.setCursor(0,1);
  lcd.print("CONTROL SYSTEM");
  servomotor.write(0);
  for(i = 3 ; i>=0 ;i--)
  {
    lcd.setCursor(14,1);
    lcd.print(">>");
    delay(1000);
  }
  lcd.clear();
  
  i = 0;  
}

void loop()
{
  if (Serial.available())
  {
   id = Serial.parseInt();
   if (id == 1)
   {
   idval = true; 
   correctid();
   
    }
   else if (id == 2)
   {
        idval = false;
        lcd.clear();
        invalidid();
   }
  }
  if (idval == true)
  {       
   passwordcode();
   endtime = millis();
   keeptime = endtime - start;
    if ( keeptime > 60000)
    {
      idval = false;
    }
  }
  else
  {
    defaultdisplay();
  }
}

int checkpassword()
{
  value =0;
  for (int k = 0; k <=5; k++)
  {
    if (input[k] == password[k])
    {
       value++; 
    }
    else
    {
      value = value;
    }
  }
  if (value == 5)
  {
    return 1;
  }
  else
  {
    return 0;
  }
  value = 0;
}

void defaultdisplay()
{
  lcd.setCursor(0,0);
  lcd.print("WELCOME!! PLEASE");
  lcd.setCursor(0,1);
  lcd.print("SCAN YOUR CARD");
  delay(500);
}

void invalidid()
{
  lcd.setCursor(0,0);
  lcd.print("INCORRECT ID");
  lcd.setCursor(0,1);
  lcd.print("USE VALID CARD");
  digitalWrite(buzzer,HIGH);
  delay(3000);
  digitalWrite(buzzer,LOW);
  lcd.clear();
  defaultdisplay();
}

void correctid()
{
  lcd.setCursor(0,0);
  lcd.print("CORRECT ID: NOW ");
  lcd.setCursor(0,1);
  lcd.print("ENTER PASSWORD");
  delay(2000);    
  lcd.clear();
  start = millis();
 }
int timeallowed = 0;
 void passwordcode()
 {
  
  lcd.setCursor(0,0);
  lcd.print("ENTER PASSWORD");
  char keypressed = mykeypad.getKey();
  if ()
  {
     input[i] = char(keypressed);
     lcd.setCursor(i,1);
     lcd.print('*');
     i++;
     if (i == 5)
     {
      int a = checkpassword();
      if (a == 1)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("ACCESS GRANTED");
        lcd.setCursor(0,1);
        lcd.print("ENTER NOW");
        digitalWrite(buzzer,LOW);
        servomotor.write(90);
          for(i = 10 ; i>=0 ;i--)
          {        
            delay(1000);
           }
           digitalWrite(buzzer,LOW);
           servomotor.write(0);
           idval = false;
        i=0;
      }
      else if (a == 0)
      {
         lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("ACCESS DENIED");
          lcd.setCursor(0,1);
           lcd.print("RE-ENTER PIN");
           digitalWrite(buzzer,HIGH);
           timeallowed++;
             for(i = 3 ; i>=0 ;i--)
            {        
              delay(1000);
             }
           i=0;
           digitalWrite(buzzer,LOW);
           if (timeallowed == 3)
           {
            idval = false;
           }
      }       
      lcd.clear();
     }
  }
  delay(100);
 }

