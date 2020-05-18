/*
  Arduino Wireless Network - Multiple NRF24L01 Tutorial
        == Node 02 (Child of Master node 00) ==    
*/
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#define BUTTON_A 6
#define BUTTON_B 7 
#define BUTTON_A_STATUS (digitalRead(BUTTON_A))
#define BUTTON_B_STATUS (digitalRead(BUTTON_B))
#define LED 5 
#define LIGHT 9

RF24 radio(8, 10);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t nodeA = 00;   // Address of our node in Octal format ( 04,031, etc)
const uint16_t nodeB = 01;
const uint16_t nodeC = 02;// Address of the other node in Octal format
const unsigned long interval = 5000;  //ms  // How often to send data to the other unit
unsigned long last_sent;
bool sendMessage = false; // When did we last send?
long data = 1;
long ledValue;
bool statusLed = false;
bool ok;

void setup() {
  SPI.begin();
  radio.begin();
  network.begin(90, nodeA);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  Serial.begin(9600);
  pinMode(BUTTON_A,INPUT);
  pinMode(BUTTON_B,INPUT);
  pinMode(LED,OUTPUT);
  pinMode(LIGHT,OUTPUT);
}
void loop() {
  network.update();
 
 while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
    unsigned long incomingData;
    network.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data
    Serial.println(incomingData);
    if (header.from_node == 1) 
    {
     statusLed = !statusLed;
     
    }
    else if (header.from_node == 2) 
    {
      ledValue = incomingData;
    }
    else{}
  }
  analogWrite(LED,ledValue);
  digitalWrite(LIGHT,statusLed);
  if(BUTTON_A_STATUS == 1)
  {
    delay(300);
    if(BUTTON_A_STATUS == 1)
    {
        RF24NetworkHeader header(nodeB);
        sendMessage = true;
         ok = network.write(header, &data, sizeof(data));
    }
  }
  if(BUTTON_B_STATUS == 1)
  {
    delay(300);
    if(BUTTON_B_STATUS == 1)
    {
        RF24NetworkHeader header(nodeC);
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
