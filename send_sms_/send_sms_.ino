#include <SoftwareSerial.h>

SoftwareSerial gsm(3,4);
//String sms;
 unsigned char buff [250];
 unsigned char buffernext;
int alertpin = 10;
int setpin = 11;
	char recipient[11] = "08161810056";//"07016717944";
	char message[15] = "HdsdsdsdELLO";
	char receivedsms[] = "ON"; 
	void setup()
	{
	  DDRB |= (1<<5);
	  Serial.begin(9600);
	  gsm.begin(9600);
	  gsm.write("AT\r\n");
	  pinMode(alertpin,INPUT_PULLUP);
	  pinMode(setpin,OUTPUT);
	   memset(buff,'\0',250);
	   buffernext = 5;  
	   gsm.write("AT+CMGF=1\r\n"); //set sms mode as text
		delay(1000);
	   gsm.write("AT+CNMI=2,2,0,0,0\r\n"); //enable auto read of incoming sms
		delay(1000);
	}

void loop() 
{  
  PORTB ^= (1<<5);
  //delay(500);
  if (Serial.available() > 0)
  {
   
    gsm.write(Serial.read());/*
    gsm.write("AT+CMGF=1\r\n");/*); //set sms mode as text
    delay(100);
    gsm.write("AT+CMGS=\"08137252680\"\r\n");delay(1000);
    gsm.write("LONGITUDEfkjlj");
    gsm.write(26);*/
    if(Serial.read() == '1')
  {
     send_sms(recipient,message);
    
  }
  
  
  }
  if (gsm.available()> 0)
  {
    //sms = gsm.read();
   // buff[buffernext] =  gsm.read();
    Serial.write(gsm.read());
  }
  if (digitalRead(alertpin) != 1)
  {
    delay(200);
     if (digitalRead(alertpin) != 1)
  {
     send_sms(recipient,message);
     delay(20);
 
  }
 }
 
} 
void receive_sms()
{
  
}
void send_sms(char *number, char *messages)
{
     gsm.write("AT+CMGF=1\r\n"); //set sms mode as text
    delay(1000);
  gsm.write("AT+CMGS=\"");
  gsm.write(number);
  gsm.write("\"");
  gsm.write("\r\n");
  delay(1000);
  gsm.write(messages);  
  delay(1000);
  gsm.write(26);

}
