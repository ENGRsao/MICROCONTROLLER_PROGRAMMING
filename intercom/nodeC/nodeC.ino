/*
  Arduino Wireless Network - Multiple NRF24L01 Tutorial
        == Node 02 (Child of Master node 00) ==    

        B -- LCD with 2 button
        A -- LED with 2 button
        C -- Serial with 1 button 1 potentimeter
*/
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#define POTENTIOMETER A1
#define BUTTON_B 7 
#define BULB (9)
#define BUTTON_B_STATUS (digitalRead(BUTTON_B))

RF24 radio(8, 10);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t nodeC = 02;   // Address of our node in Octal format ( 04,031, etc)
const uint16_t nodeA = 00;
const uint16_t nodeB = 01;// Address of the other node in Octal format
const unsigned long interval = 5000;  //ms  // How often to send data to the other unit
unsigned long last_sent;
bool sendMessage = false; // When did we last send?
long data = 1;
unsigned long potentiometer_Val1 ,potentiometer_Val2 ;
int serialData ;
bool ok;
bool bulbState = false;
void setup() {
  SPI.begin();
  radio.begin();
  network.begin(90, nodeC);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  Serial.begin(9600);
  pinMode(BULB,OUTPUT);
  pinMode(BUTTON_B,INPUT);
  pinMode(POTENTIOMETER,INPUT);
    unsigned long potValue = analogRead(POTENTIOMETER);
    potentiometer_Val1  = map(potValue, 0, 1023, 0, 255);
  
}
void loop() {
  network.update();
  if(Serial.available() > 0)
  {
    serialData  = Serial.parseInt();
//     while(Serial.available() > 0)
//     {
//       SerialData += Serial.read();
//     }     
  }
 while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
    unsigned long incomingData;
    network.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data
    Serial.println(incomingData);
    if (header.from_node == 0) 
    {
      Serial.print("Message Received from Node A");
    }
    else if(header.from_node == 1) 
    {
       Serial.print("Message Received from Node B");
    }
    else{}
    bulbState = !bulbState;
  }
    digitalWrite(BULB,bulbState);
    unsigned long potValue = analogRead(POTENTIOMETER);
    potentiometer_Val2  = map(potValue, 0, 1023, 0, 255);
    //Serial.println(potentiometer_Val2);
  if((unsigned long)(potentiometer_Val2 - potentiometer_Val1) > 5)
  {
      RF24NetworkHeader header(nodeA);
      data = potentiometer_Val2;
      potentiometer_Val1 = potentiometer_Val2;
      sendMessage = true;
      ok = network.write(header, &data, sizeof(data));
    }
  if(BUTTON_B_STATUS == 1)
  {
    delay(300);
    if(BUTTON_B_STATUS == 1)
    {
        RF24NetworkHeader header(nodeB);
        data = 1;
        sendMessage = true;
        ok = network.write(header, &data, sizeof(data));
    }
  }

  if(sendMessage)
  {
     
     if(ok) Serial.print("sent");
     else Serial.print("unsent");
     sendMessage = false;
  }
  
 }
