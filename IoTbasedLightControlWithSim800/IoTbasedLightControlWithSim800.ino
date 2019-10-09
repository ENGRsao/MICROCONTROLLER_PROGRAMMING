#include <SoftwareSerial.h>
 
//SIM800 TX is connected to Arduino D8
#define SIM800_TX_PIN 8
 
//SIM800 RX is connected to Arduino D7
#define SIM800_RX_PIN 7
 
//Create software serial object to communicate with SIM800
SoftwareSerial sms (SIM800_TX_PIN,SIM800_RX_PIN);
 unsigned char buff [250];
 unsigned char buffernext;
 int ldr = A0;
 int LDRvalue;
 int bulb = 4;
 int selectoption = 13;
 int i =0;
 
void setup() {
  //Begin serial comunication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  sms.begin(9600);
    delay(1000);
  //Being serial communication with Arduino and SIM800
  Serial.println(" STREET LIGHT CONTROL VIA SMS ");
 //Set SMS format to ASCII 
 sms.write("AT+CMGF=1\r\n");//set sms mode as text
  Serial.println("AT+CMGF=1\r\n");
  delay(1000);
   
//gsm.SimpleWriteln("AT+CNMI=2,2,0,0,0");
  sms.write("AT+CNMI=2,2,0,0,0\r\n"); //enable module to auto-read all incoming message
  Serial.println("AT+CNMI=2,2,0,0,0\r\n");
  delay(1000);
 //  Serial.write(sms.read());
  pinMode(13,OUTPUT);
  pinMode(bulb,OUTPUT);
  pinMode(ldr,INPUT);
  pinMode(selectoption,INPUT_PULLUP);
  pinMode(bulb,LOW);
  memset(buff,'\0',250);
  buffernext = 5;
  
 }
 
void loop() {  
 
  if (digitalRead(selectoption) == 1)
  {
        if (sms.available()>0) //check if sms is comming in
        {       
            buff[buffernext] =  sms.read();
             Serial.write(buff[buffernext]);
               if (buff[buffernext] == 'N')
               {            
                 if((buff[buffernext-1] == 'O')&&(buff[buffernext-2] == '@'))
               {         
                  Serial.print("&");
                  digitalWrite(bulb,HIGH);
                  Serial.println("ON LED");
               }           
               }
               
               if (buff[buffernext] == 'F')
               {            
                 if((buff[buffernext-1] == 'F')&&(buff[buffernext-2] == 'O'))
               {                     
                digitalWrite(bulb,LOW);
                Serial.println("OFF LED");
               }           
               }
                
               buffernext++;
                if (buffernext >250)
                {
                  buffernext = 5;
                }
          }
      
  }
          else
          {
            i = 0;
           LDRvalue=analogRead(ldr);
              Serial.println(LDRvalue);
              if(LDRvalue >= 200)
                 {
                   digitalWrite(bulb,HIGH);
                   delay(500);
                 }
              else if(LDRvalue < 200)
              {
                digitalWrite(bulb,LOW);
                delay(500);
              }
      }
}
 
