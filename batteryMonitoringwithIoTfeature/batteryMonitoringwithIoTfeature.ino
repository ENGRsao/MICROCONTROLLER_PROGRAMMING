#include <ESP8266WiFi.h>
#define BATTERY_100 110.40
#define BATTERY_75 104.8
#define BATTERY_50  99.0
#define BATTERY_25  93.6
#define BATTERY_0  88.0
String apiWritekey = "7ZT5MD6WP6SCXW4Z"; // replace with your THINGSPEAK WRITEAPI key here
const char* ssid = "Photizzo_Office"; // your wifi SSID name
const char* password = "1r0bun007&008" ;// wifi pasword

const char* server = "api.thingspeak.com";
float resolution = 3.3 / 1023.0; // 3.3 is the supply volt  & 1023 is max analog read value
WiFiClient client;
int serverPort = 25;
float batteryV ; 
char message[15] = "Hello,GOOD" ;
String mailserver  = "www.gmail.com", username = "akeemolalekan229@gmail.com", passwords = "Abdulakeem25_", senderMail = "akeemolalekan229@gmail.com", senderName = "Akeem", recipientEmail = "aosafiriyu@gmail.com", recipientName = "SAO", subject = "testMail", recipient_number = "+2348137252680";
void setup() {
  Serial.begin(115200);
  WiFi.disconnect();
  delay(10);
  WiFi.begin(ssid, password);

  // Serial.println();
  //Serial.println();
  //Serial.print("Connecting to ");
  //Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  //Serial.println("");
  //Serial.print("NodeMcu connected to wifi...");
  //Serial.println(ssid);
  //Serial.println();
  Serial.print("AT\r\n");
}

void loop() {
  batteryV = (analogRead(A0) * resolution);

    if (client.connect(server,80))
    {
    String tsData = apiWritekey;
           tsData +="&field1=";
           tsData += String(batteryV);
           tsData += "\r\n\r\n";

     client.print("POST /update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("X-THINGSPEAKAPIKEY: "+apiWritekey+"\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(tsData.length());
     client.print("\n\n");  // the 2 carriage returns indicate closing of Header fields & starting of data
     client.print(tsData);

     //Serial.print("Battery Voltage: ");
     //Serial.print(batteryV);
     //Serial.println("uploaded to Thingspeak server....");
    }
    client.stop();

    //Serial.println("Waiting to upload next reading...");
    //Serial.println();
    // thingspeak needs minimum 15 sec delay between updates
  
  
  batteryRange();
  delay(10000);
  batteryRange();
  delay(10000);
  batteryRange();
  delay(10000);
}

void batteryRange(void)
{
  if (batteryV == BATTERY_100)
  {
    String message = "BATTERY @ 100%%";
      sendSMS(message);
      delay(2000);
      sendEmail(message);
  }
   else if (batteryV == BATTERY_75)
  {
  String message = "BATTERY @ 75%%";
      sendSMS(message);
      delay(2000);
      sendEmail(message);
  }
   else if (batteryV == BATTERY_50)
  {
      String message = "BATTERY @ 50%%";
      sendSMS(message);
      delay(2000);
      sendEmail(message);
  }
   else if (batteryV == BATTERY_25)
  {
  String message = "BATTERY @ 25%%";
      sendSMS(message);
      delay(2000);
      sendEmail(message);
  }
   else if (batteryV == BATTERY_0)
  {
   String message = "BATTERY @ 0%%";
      sendSMS(message);
      delay(2000);
      sendEmail(message);
  }
  else{}
}

void sendEmail(String mes)
{
  Serial.write("AT+SAPBR = 3,1,\"web.gprs.mtnnigeria.com\",\"web\""); Serial.write("\r\n");  //set up gprs
  delay(60);
  Serial.write("AT+SAPBR = 1,1"); Serial.write("\r\n"); //OPEN PDP
  delay(60);
  Serial.write("AT+SAPBR=2,1"); Serial.write("\r\n"); //IP ADDRESS
  delay(60);
  Serial.write("AT+EMAILCID = 1"); Serial.write("\r\n");
  delay(60);
  Serial.write("AT+EMAILTO = 30"); Serial.write("\r\n");
  Serial.write("\"");
  delay(60);
  Serial.write("AT+SMTPSRV = "); Serial.write("\"");Serial.print(mailserver); Serial.write("\"");Serial.write(','); Serial.write("\"");Serial.print(serverPort); Serial.write("\"");Serial.write("\r\n"); //"WWW.GMAIL.COM",25
  delay(60);
  Serial.write("AT+SMTPAUTH=1,"); Serial.write("\"");Serial.print(username); Serial.write("\"");Serial.write(','); Serial.write("\"");Serial.print(passwords);Serial.write("\""); Serial.write("\r\n");
  delay(60);
  Serial.write("AT+SMTPFROM = ");Serial.write("\""); Serial.print(senderMail); Serial.write("\"");Serial.write(','); Serial.write("\"");Serial.print(senderName);Serial.write("\""); Serial.write("\r\n"); // "EMAIL,USERNAME
  delay(60);
  Serial.write("AT+SMTPRCPT = 0,0,"); Serial.write("\"");Serial.print(recipientEmail);Serial.write("\""); Serial.write(','); Serial.write("\"");Serial.print(recipientName); Serial.write("\"");Serial.write("\r\n");
  delay(60);
  Serial.write("AT+SMTPSUB = "); Serial.write("\"");Serial.print(subject); Serial.write("\"");Serial.write("\r\n");
  delay(60);
  Serial.write("AT+SMTPBODY"); Serial.write("\r\n");
  delay(60);Serial.write("\"");
  Serial.print(mes); Serial.write("\"");Serial.write("\r\n"); delay(100);Serial.println(char(26)); Serial.write("\r\n");
  
  Serial.write("AT+SMTPSEND");
}
void sendSMS(String mes)
{
  /*Serial.write("AT+CMGF=1"); Serial.write("\r\n");
  delay(200);
  Serial.write("AT+CMGS="); Serial.print(recipient_number); Serial.write("\r\n");
  delay(80);
  Serial.print(message);   delay(30);Serial.write(26); Serial.write("\r\n");*/
  
Serial.println("AT+CMGF=1");
  delay(1000);
 Serial.println("AT+CMGS=\"" + recipient_number + "\"\r");
  delay(1000);
  //String SMS = "How are you?";
 Serial.println(mes);
  delay(100);
Serial.println(char(26));
  delay(1000);
}
