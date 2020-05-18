/*
  Arduino Wireless Network - Multiple NRF24L01 Tutorial
        == Node 02 (Child of Master node 00) ==
*/
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <Servo.h>
#define led 2
RF24 radio(8, 10);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 00;   // Address of our node in Octal format ( 04,031, etc)
const uint16_t master00 = 02;    // Address of the other node in Octal format
Servo myservo;  // create servo object to control a servo
void setup() {
  SPI.begin();
  radio.begin();
  Serial.begin(9600);
  network.begin(90, this_node); //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  myservo.attach(3);   // (servo pin)
  pinMode(led, OUTPUT);
}
void loop() {
  network.update();
  //===== Receiving =====//
  while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
    unsigned long incomingData;
    network.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data
    Serial.println(incomingData);
//    if (header.from_node == 0) {    // If data comes from Node 02
//      myservo.write(incomingData);  // tell servo to go to a particular angle
//    }
//    if (header.from_node == 10) {    // If data comes from Node 012
//      digitalWrite(led, !incomingData);  // Turn on or off the LED 02
//    }
  }
}