 

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
//#include <SD.h>
#include "SdFat.h"

using namespace sdfat;
char data[50];
SdFat SD;

#define SD_CS_PIN 4
#define LED D8
File myFile;

// Update these with values suitable for your network.

const char* ssid = "Photizzo_Office";//"SAOSIR";
const char* password = "1r0bun007&008";//"akddmzao";
const char* mqtt_server = "m12.cloudmqtt.com";//"m23.cloudmqtt.com";
const int mqttPort =  11397;//17700;
const char* mqttUser = "frppzfyn";//"wwrivypi";
const char* mqttPassword = "bMdRIPKsq6bC";//"C6r-EMOfyxb4";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
uint8_t qos = 1;
bool retained =  true;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
 
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}

void reconnect() {
  // Loop until we're reconnected
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESPD8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect("ESPD8266Client", mqttUser, mqttPassword)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world",qos,retained);
      // ... and resubscribe
      client.subscribe("inTopic",qos);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
bool serialdata, ledstate = false;
void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqttPort);
  client.setCallback(callback);
   pinMode(BUILTIN_LED,OUTPUT);
    if (!SD.begin(SD_CS_PIN)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}
int i = 0;
void loop() {
  if(Serial.available() > 0)
  {
    while(Serial.available() > 0)
    {
      char a =  (Serial.read());
       data[i++] = a;       
    }
    serialdata = true;
       i = 0;
  }

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    ledstate = !ledstate;
   digitalWrite(BUILTIN_LED,ledstate);
  }
    Serial.print("Publish message: ");
    Serial.println(msg);
    if (serialdata)
    { 
      if (client.connected())
      {
        client.publish("WATTBANK S", data,retained);
        Serial.print("Published message: ");
      }
      else
      {
        myFile = SD.open("data.txt", FILE_WRITE);
        if (myFile) {
          Serial.print("Writing available data to sd card");
          myFile.println(data);
          myFile.close();
          Serial.println("done.");
        } 
        else {
          // if the file didn't open, print an error:
          Serial.println("error opening test.txt");
       }        
      }
      serialdata = false;
      for (int i = 0;i<50;i++)
      {
        data[i] = '\0';
      }
    }
 
}
