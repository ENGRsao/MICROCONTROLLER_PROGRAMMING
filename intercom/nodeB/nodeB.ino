/*
  Arduino Wireless Network - Multiple NRF24L01 Tutorial
        == Node 02 (Child of Master node 00) ==    
*/
#include <RF24Network.h>
#include <LiquidCrystal.h>
#include <RF24.h>
#include <SPI.h>
#define BUTTON_A 6
#define BUTTON_B 7 
#define BUTTON_A_STATUS (digitalRead(BUTTON_A))
#define BUTTON_B_STATUS (digitalRead(BUTTON_B))

RF24 radio(8, 10);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
LiquidCrystal lcd(14,15,16,17,18,19);
const uint16_t nodeB = 01;   // Address of our node in Octal format ( 04,031, etc)
const uint16_t nodeA = 00;
const uint16_t nodeC = 02;// Address of the other node in Octal format
const unsigned long interval = 5000;  //ms  // How often to send data to the other unit
unsigned long last_sent;
bool sendMessage = false; // When did we last send?
long data = 1;
bool ok;
void setup() {
  SPI.begin();
  radio.begin();
  network.begin(90, nodeB);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  Serial.begin(9600);
  pinMode(BUTTON_A,INPUT);
  pinMode(BUTTON_B,INPUT);
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("WAITING FOR DATA");
  delay(2000);
  lcdHome();
}
void loop() {
  network.update();
 
 while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
    unsigned long incomingData;
    network.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data
    Serial.println(incomingData);
    if (header.from_node == 0) 
    {
      printData("A");
    }
    else if (header.from_node == 2)    {
      printData("C");
    }
   else{}
  }

  if(BUTTON_A_STATUS == 1)
  {
    delay(300);
    if(BUTTON_A_STATUS == 1)
    {
        RF24NetworkHeader header(nodeA);
        sendMessage = true;
        ok = network.write(header, &data, sizeof(data));
        printSend("A");
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
        printSend("C");
    }
  }

  if(sendMessage)
  {
     
     if(ok) Serial.print("sent");
     else Serial.print("unsent");
     sendMessage = false;
  }
  
 }

 void printData(String val)
 {
  //lcd.clear();
  lcd.setCursor(12,0);
  lcd.print("  ");
  
  lcd.setCursor(12,0);
  lcd.print(val);
 }

 
 void printSend(String val)
 {
  //lcd.clear();
  lcd.setCursor(10,1);
  lcd.print("  ");
  
  lcd.setCursor(10,1);
  lcd.print(val);
 }
 void lcdHome()
 {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("DATA FROM:");
  
  lcd.setCursor(1,1);
  lcd.print("SENT TO:");
 }
