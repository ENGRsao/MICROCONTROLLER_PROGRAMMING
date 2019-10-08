#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h> 

#include <SPI.h>
#include <SD.h>

#include <SoftwareSerial.h>
#define rxd 2
#define txd 3
SoftwareSerial  bluetooth(rxd,txd);
int bluetoothvalue;

File sdcard;
unsigned long starttime = 0,endtime = 0; 
unsigned long  id=0;

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

int conduct = A0,conductval;

int DHpin = 8;
int humidval,humiddec,tempval,tempdec;
byte dat [5];
byte read_data () {
  byte data;
  for (int i = 0; i < 8; i ++) {
    if (digitalRead (DHpin) == LOW) {
      while (digitalRead (DHpin) == LOW); // wait for 50us
      delayMicroseconds (30); // determine the duration of the high level to determine the data is '0 'or '1'
      if (digitalRead (DHpin) == HIGH)
        data |= (1 << (7-i)); // high front and low in the post
      while (digitalRead (DHpin) == HIGH); // data '1 ', wait for the next one receiver
     }
  }
return data;
}
 
void start_test () {
  digitalWrite (DHpin, LOW); // bus down, send start signal
  delay (30); // delay greater than 18ms, so DHT11 start signal can be detected 
  digitalWrite (DHpin, HIGH);
  delayMicroseconds (40); // Wait for DHT11 response 
  pinMode (DHpin, INPUT);
  while (digitalRead (DHpin) == HIGH);
  delayMicroseconds (80); // DHT11 response, pulled the bus 80us
  if (digitalRead (DHpin) == LOW);
  delayMicroseconds (80); // DHT11 80us after the bus pulled to start sending data 
  for (int i = 0; i < 4; i ++) // receive temperature and humidity data, the parity bit is not considered
    dat[i] = read_data (); 
  pinMode (DHpin, OUTPUT);
  digitalWrite (DHpin, HIGH); // send data once after releasing the bus, wait for the host to open the next Start signal
}

void setup() {
  // put your setup code here, to run once:  
  
   Serial.begin(9600);
   pinMode (DHpin, OUTPUT);
   bluetooth.begin(9600); 
   
 Wire.begin();
 lcd.begin (16,2);  // initialize the lcd
 lcd.clear();
 lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
 lcd.setBacklight(LED_ON);
     lcd.setCursor(0,0);
    lcd.print("WATER QUALITY");
    lcd.setCursor(0,1);
    lcd.print("MONITORING");
    delay(3000);
 
 pinMode(conduct,INPUT);
 
  if (!SD.begin(10)) {
     lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SD CARD ABSENT");
    delay(1000);
    //return;
  }
  sdcard = SD.open("Data.csv", FILE_WRITE);
 if(sdcard)
{
  sdcard.println(", , , ,");
  String start = "ID , CONDUCTIVITY, TEMPERATURE, HUMIDITY";
  sdcard.println(start);
  sdcard.close();
   lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("File Opened");
  delay(1000);
 }
 else
 {
   lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Fail to Open file");
  delay(1000);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PLACE SENSOR IN");
   lcd.setCursor(0,1);
  lcd.print("THE WATER");
  delay(3000);
  starttime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  conductval = map(analogRead(conduct),0,1023,0,100);  
  start_test ();
  humidval = int(dat[0]);
  tempval =  int(dat[2]);
 
  if((millis() - starttime)>5000)
  {
   id++;

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print ("HUM:");
  lcd.setCursor(4,0);
  lcd.print (dat[0], DEC); // display the humidity-bit integer;
  lcd.setCursor(7,0);
  lcd.print ('%');
  lcd.setCursor(8,0);
  lcd.print ("CON:");
  lcd.setCursor(12,0);
  lcd.print (conductval);
  lcd.setCursor(15,0);
  lcd.print ('%');
  lcd.setCursor(0,1 );
  lcd.print ("TEMPERATURE:");
  lcd.setCursor(12,1);
  lcd.print (dat[2], DEC); // display the temperature of integer bits;
  lcd.setCursor(15,1);
  lcd.print("C");
   bluetooth.print (int(dat [2]));
   bluetooth.print ("|");
   bluetooth.print (int(dat [0]));
   bluetooth.print ("|");
   bluetooth.print (int(conductval));
   bluetooth.print ("|");
   Serial.print (dat [2],DEC);
   Serial.println ("|");
   Serial.print (dat [0],DEC);
   Serial.println ("|");
   Serial.print ((conductval));
   Serial.println ("|");

   sdcard = SD.open("Data.csv", FILE_WRITE);
    if(sdcard)
      {
        sdcard.println(" , , , ,");
        String start = String(id)+ " ,"+  String(conductval)+ " ," +  String(tempval) + " ," +  String(humidval);
        sdcard.println(start);
        sdcard.close();
        delay(1000);
        starttime = millis();
      }
      else
      {
        Serial.println("Couldnot open file");
      }
  }
  
    delay(1000);
}
