#include <SoftwareSerial.h>

SoftwareSerial SIM800(6,7);
int sim_reset = 4;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
SIM800.begin(9600);
pinMode(sim_reset,OUTPUT);
//check if sim is connected
SIM800.write("AT\r\n");
delay(10);
Serial.print(SIM800.read());
//check the current timing of the sim module
SIM800.write("AT+CCLK?\r\n");
delay(50);
Serial.print(SIM800.read());
//synchronize time with the current time
SIM800.write("AT+CLTS = 1\r\n");
delay(50);
Serial.print(SIM800.read());
SIM800.write("AT&W\r\n");
delay(50);
Serial.print(SIM800.read());

//reset sim module for it to get synchronized
digitalWrite(sim_reset,HIGH);
delay(2500);
digitalWrite(sim_reset,LOW);
delay(2500);

//sim clock synchronized.. print out the current time 
SIM800.write("AT+CCLK?\r\n");
delay(50);
Serial.print(SIM800.read());
}

void loop() {
  // put your main code here, to run repeatedly:

}
