#include <SoftwareSerial.h>

SoftwareSerial sim(3,4);


const int echoPin = 5;
const int triggerPin = 6;
int maxLevel = 1200;

int led = 13,_sim_status=0,xmit=0;
String sim_apn = "web.mtnnigeria.com",port_no = "80",ip_address = "api.thingspeak.com",API_KEY ="0W2WC1BBPM74DN0L",field= "field1=";
int watersensor=A1, water_value;String reply;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
 pinMode(echoPin,INPUT);
 pinMode(triggerPin,OUTPUT);
 Serial.println("Setting up SIM module");
 sim.begin(9600);
 pinMode(led,OUTPUT);
 delay(3000);
 //confirmSim();
 Serial.println(data_received());
}

unsigned long timea = millis();

void loop() {
  // put your main code here, to run repeatedly:
unsigned long timeb = millis();
if (xmit == 0)
{ sim_debug();}

if ((unsigned long)(timeb - timea) > 5000)
{
  water_value = getWaterLevel();
  Serial.println(water_value);
  sendData(API_KEY,field,String(water_value));
  delay(1000);
  sim_debug();
  timea = millis();
}

}

float getWaterLevel()
{
digitalWrite(triggerPin, LOW);
delayMicroseconds(2);
digitalWrite(triggerPin, HIGH);
delayMicroseconds(1);
digitalWrite(triggerPin, LOW);
int echoTime = pulseIn(echoPin,HIGH);
int level = echoTime/6;  //converts the ultrasonic readings to millimeter
if(level > 0)
{
  int waterLevel = maxLevel - level;
  if(waterLevel > 0){
    return waterLevel;
  }  
  else{
    return 0;
  } 
  }
else{
  return 0;
  }
}


void confirmSim()
{
   sim.listen();
      delay(10);
    sim.write("AT\r\n");
    delay(50);
    while(1)
    {
      if (sim.find("OK"))
      {
        break;
      }
      Serial.println("NO SIM");
      blink_led(led);
      sim.write("AT\r\n");
    delay(200);
    }
}

void shutConnection()
{
  sim.write("AT+CIPSHUT\r\n");
  delay(1000);
  Serial.println(data_received());
}

void setConnectiontype()
{
    sim.write("AT+CIPMUX=0\r\n");
    delay(1000);
    Serial.println(data_received());
    sim.write("AT+CGATT=1\r\n");
    delay(400);
    Serial.println(data_received());
    sim.write("AT+CSTT=\"");sim.print(sim_apn);sim.write("\",\"\",\"\"\r\n");
    delay(1000);
    Serial.println(data_received());
}

void startWireless()
{
  sim.write("AT+CIICR\r\n");
  delay(1000);
  Serial.println(data_received());
  
}

void startTCP()
{
  sim.write("AT+CIPSTART=\"TCP\",\"");sim.print(ip_address);sim.write("\",\"");sim.print(port_no);sim.write("\"\r\n");
  delay(1000);
  Serial.println(data_received());
}
void stopTCP()
{
  sim.write("AT+CIPCLOSE\r\n");
  delay(500);
  Serial.println(data_received());
}
void sendData(String channel_key,String field,String message)
{  
  sim.write("AT+CIPSEND\r\n");
  delay(100);
  Serial.println(data_received());
  sim.write("GET https://api.thingspeak.com/update?api_key=");sim.print(channel_key);sim.write("&");sim.print(field);sim.print(message);sim.write("\r\n");
  delay(100);
  sim.write(0x1A);
  delay(500);
  
  Serial.println(data_received());
}
void sim_status()
{
  sim.listen();
  delay(10);  
    sim.write("AT+CIPSTATUS\r\n");
    delay(500);
    Serial.println(data_received());   
    if (check_response("IP INITIAL",reply)==1)
    {
      _sim_status=0; 
    }else if (check_response("IP START",reply)==1)
    {
       _sim_status=1; 
    }else if (check_response("IP CONFIG",reply)==1)
    {
       _sim_status=2; 
    }else if (check_response("IP GPRSACT",reply)==1)
    {
       _sim_status=3; 
    }else if (check_response("IP STATUS",reply)==1)
    {
       _sim_status=4; 
    }else if (check_response("TCP CONNECTING",reply)==1)
    {
       _sim_status=5; 
    }else if (check_response("CONNECT OK",reply)==1)
    {
       _sim_status=6; 
    }else if (check_response("TCP CLOSING",reply)==1)
    {
       _sim_status=7; 
    }else if (check_response("TCP CLOSED",reply)==1)
    {
       _sim_status=8; 
    }else if (check_response("PDP DEACT",reply)==1)
    {
       _sim_status=9; 
    }         
}
//------------------------------------------------------------------------------------------
void sim_debug()
{
  sim.listen();
  delay(10);
   
  sim_status();
  
  if (_sim_status==0)  // status:IP INITIAL. Shut IP. Set APN. 
  {
    shutConnection();
    setConnectiontype();
    sim_status();
    digitalWrite(led,HIGH);
  }
  if (_sim_status==1)  // status:IP START. Activate GPRS connection
  {
    xmit=0;
    startWireless();
    sim_status();
    digitalWrite(led,HIGH);
  }
  if (_sim_status==3)  // status:IP GPRSACT. Get IP 
  {
    xmit=0;
    sim_ip();
    sim_status();
    digitalWrite(led,HIGH);
  }
  if (_sim_status==4)  // status:IP STATUS. Start TCP connection
  {
    xmit=0;
    startTCP();
    sim_status();
    digitalWrite(led,HIGH);
  }
  if (_sim_status==5)  // status:TCP CONNECTING. Wait for TCP connection
  {
    xmit=0;
    digitalWrite(led,HIGH);
  }
  if (_sim_status==6)  // status:CONNECT OK. Set transmit flag 
  {
    if (xmit==0)
    {
      startTCP();
      xmit=1;
     }
   blink_led(led);
  }
  if (_sim_status==7)  // status:TCP CLOSING. Clear transmit flag. 
  {
    xmit=0;
    sim_status();
    digitalWrite(led,HIGH);
  }
  if (_sim_status==8)  // status:TCP CLOSED. Restart TCP connection. 
  {
    xmit=0;
    startTCP();
    digitalWrite(led,HIGH);
  }
  if (_sim_status==9)  // status:PDP DEACT. Shut IP.
  {
    xmit=0;
    shutConnection();
    digitalWrite(led,HIGH);
  }
  
}
void sim_ip()
{
  sim.write("AT+CIFSR\r\n");
  delay(1000);
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
short check_response(String query,String source)
{
  short stat=0;
  int query_len=query.length();

  for (int i=0;i<(source.length());i++)
  {
    if (source.substring(i,i+query_len) == query) 
    {
      stat=1;
    }
  }
  return stat;
}
void blink_led(int a)
{
  digitalWrite(a,HIGH);
  delay(100);
  digitalWrite(a,LOW);
  delay(100);
 }
