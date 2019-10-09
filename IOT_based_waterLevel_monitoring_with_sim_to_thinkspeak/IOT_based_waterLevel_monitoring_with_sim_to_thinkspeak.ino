#include <SoftwareSerial.h>

SoftwareSerial sim(10,11);

String sim_apn = "web.mtnnigeria.com",port_no = "80",ip_address = "api.thingspeak.com",API_KEY ="0W2WC1BBPM74DN0L",field= "field1=";
int watersensor=A1, water_value;String reply;
void setupSim();
void startTCP();

void setup() {
  // put your setup code here, to run once:
 
 Serial.begin(9600);
 Serial.println("Setting up SIM module");
 sim.begin(9600);
 delay(3000);
 Serial.println(data_received());
 setupSim();
 
 startTCP();
 Serial.println(data_received());
}

void loop() {
  // put your main code here, to run repeatedly:
water_value = 50;//map(analogRead(watersensor),0,1023,0,100);
Serial.println(water_value);
sim_status();
startTCP();
sendData(API_KEY,field,String(water_value));
stopTCP();
delay(1000);
}

void setupSim()
{  
  sim.listen();
  delay(10);
sim.write("AT\r\n");
delay(200);
Serial.println(data_received());
sim.write("AT+CIPSHUT\r\n");
delay(1000);
Serial.println(data_received());

sim.write("AT+CIPMUX=0\r\n");
delay(1000);
Serial.println(data_received());
sim.write("AT+CGATT=1\r\n");
delay(1000);
Serial.println(data_received());
sim.write("AT+CSTT=\"");sim.print(sim_apn);sim.write("\",\"\",\"\"\r\n");
delay(1000);
Serial.println(data_received());
sim.write("AT+CIICR\r\n");
delay(1000);
Serial.println(data_received());
sim.write("AT+CIFSR\r\n");
delay(1000);
Serial.println(data_received());
}

void startTCP()
{
sim.write("AT+CIPSTART=\"TCP\",\"");sim.print(ip_address);sim.write("\",\"");sim.print(port_no);sim.write("\"\r\n");
delay(100);
Serial.println(data_received());
}
void stopTCP()
{
sim.write("AT+CIPCLOSE\r\n");
delay(100);
Serial.println(data_received());
}
void sendData(String channel_key,String field,String message)
{
  
  sim.write("AT+CIPSEND\r\n");
  delay(10);
  sim.write("GET/update?api_key=");sim.print(channel_key);sim.write("&");sim.print(field);sim.print(message);sim.write("\r\n");
  delay(100);
Serial.println(data_received());
}

void get_field_data(String field,String channel_key,String field_no,String entry_no)
{
  sim.write("AT+CIPSEND\r\n");
  delay(10);
  sim.write("GET /channels/553791/fields/");sim.print(field_no);sim.write(".json?api_key=");sim.print(channel_key);sim.write("&results=");sim.print(entry_no);sim.write("\r\n");
  delay(100);
  Serial.println(data_received());
}

void get_allfields_data(String channel_key,String entry_no)
{
  sim.write("AT+CIPSEND\r\n");
  delay(10);
  sim.write("GET /channels/553791/feeds.json?api_key=");sim.print(channel_key);sim.write("&results=");sim.print(entry_no);sim.write("\r\n");
  delay(100);
  Serial.println(data_received());
}

void get_fields_status(String channel_key)
{
  sim.write("AT+CIPSEND\r\n");
  delay(10);
  sim.write("GET /channels/553791/status.json?api_key=");sim.print(channel_key);sim.write("\r\n");
  delay(100);
  Serial.println(data_received());
}
void sim_status()
{
  sim.write("AT+CIPSTATUS\r\n");
  delay(10);
  Serial.println(data_received());
}
String data_received()
{
  reply = "";char data ="";
  sim.listen();
  delay(10);
  while(sim.available()>0)
  {
    data = sim.read();
    reply +=data;
  }
  return reply;
}

