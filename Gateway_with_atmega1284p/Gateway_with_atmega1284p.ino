
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"

RTC_DS1307 rtc;
SoftwareSerial FTDI(0,1);
SoftwareSerial SIM(10,11);
SoftwareSerial power(21,22);   //PC
SoftwareSerial control(13,14);  //Control board

#define led_esp 2
#define led_sim 12
#define led_sd 3
#define config_but 23
#define timeout 20000
#define ctrl "CONTROL"
#define pc "POWER"

char buffer[20];

enum {ssid, passwrd,ip,hotspot,hotspot_pass};

 const char esp_ssid[] PROGMEM = "PowerStore1";
 const char esp_password[] PROGMEM=  "12345678";
 const char esp_ip[] PROGMEM = "192.168.1.1";
 const char esp_hotspot[] PROGMEM ="TP-Link_B332";
 const char esp_hotspot_pass[] PROGMEM ="thinkact&lead";
 const char* const esp_data[] PROGMEM = {esp_ssid,esp_password,esp_ip,esp_hotspot,esp_hotspot_pass};

  enum {e_check,e_reset,e_server,e_mode,e_disconnect,e_type,e_ip,e_config,e_send,e_status,e_start,e_join};
 
  const char espcheck[] PROGMEM = "AT";
  const char espreset[] PROGMEM = "AT+RST";
  const char esp_create_server[] PROGMEM = "AT+CIPSERVER=1,80";
  const char esp_mode[] PROGMEM = "AT+CWMODE";
  const char esp_disconnect[] PROGMEM = "AT+CWQAP";
  const char esp_connectiontype[] PROGMEM = "AT+CIPMUX=1";
  const char esp_ip_address[] PROGMEM = "AT+CIPAP";
  const char esp_config[] PROGMEM = "AT+CWSAP";
  const char esp_send[] PROGMEM = "AT+CIPSEND";
  const char espstatus[] PROGMEM = "AT+CIPSTATUS";
  const char esp_start[] PROGMEM = "AT+CIPSTART=\"TCP\",";
  const char esp_join[] PROGMEM = "AT+CWJAP";
  const char* const esp_commands[] PROGMEM = {espcheck,espreset,esp_create_server,esp_mode,esp_disconnect,esp_connectiontype,esp_ip_address,esp_config,esp_send,espstatus,esp_start,esp_join};
  
  enum {s_check,s_reset,s_apn,s_gprs,s_ssl,s_shut,s_start,s_config,s_status,s_send};
  
  const char simcheck[]PROGMEM = "AT";
  const char simreset[]PROGMEM = "AT+RST";
  const char simapn[]PROGMEM = "AT+CSTT";
  const char simgprs[] PROGMEM = "AT+CIICR";
  const char simssl[] PROGMEM = "AT+CIPSSL";
  const char simshut[] PROGMEM = "AT+CIPSHUT";
  const char simstart[] PROGMEM = "AT+CIPSTART=\"TCP\",";
  const char simconfig[] PROGMEM = "AT+CIFSR";
  const char simstatus_check[] PROGMEM = "AT+CIPSTATUS";
  const char sim_send[] PROGMEM = "AT+CIPSEND";
  const char* const sim_commands[] PROGMEM = {simcheck,simreset,simapn,simgprs,simssl,simshut,simstart,simconfig,simstatus_check,sim_send};
  

String config_data[10] = {"","","","","","","","","",""};

String serial_no="", sim_apn="", sim_user="",sim_pwd="",server_url="",port="",password="", username="",message="",reply="",ssl_port="";
uint8_t _sim_status=0,_esp_status, xmit=0,e_xmit = 0, _priority=0;
int esp_pingtime=0, sim_pingtime=0;
uint8_t data_len;
int count= 0;
String file="";
String time_stamp = "";

char* get_Rom_Data(const char* Table_Details[],int i)
{
   strcpy_P(buffer,(char*) pgm_read_word(&Table_Details[i]));
   return buffer;
}

void setup() {
  // put your setup code here, to run once:
  //set up BAUDRATE
   
  FTDI.begin(9600);
  Serial.begin(9600);
  SIM.begin(9600);
  power.begin(9600);
  control.begin(9600);

  //configure all used pin as input 
  pinMode(31,INPUT);
  pinMode(30,INPUT);
  pinMode(29,INPUT);
  pinMode(28,INPUT);
  pinMode(27,INPUT);
  pinMode(26,INPUT);
  pinMode(25,INPUT);
  pinMode(24,INPUT);
  pinMode(20,INPUT);
  pinMode(19,INPUT);
  pinMode(18,INPUT);
  pinMode(15,INPUT);
  //RTC configuration
  rtc.begin();
  delay(10);
  if (! rtc.isrunning()) {
    FTDI.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
     //This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
     rtc.adjust(DateTime(2018,8,16,1,30,30));
  }
  else
  {
    FTDI.print("TIME:= ");
    FTDI.println(get_currentTime());
  }
  //configure debugging LED
  pinMode(led_esp,OUTPUT);
  pinMode(led_sim,OUTPUT);
  pinMode(led_sd,OUTPUT);
  delay(2000);
    
  first_boot_config();   // check for configuration mode
    
 FTDI.print("Initializing SD card...");
 int res = SD_init(); //check for SD card
 while (1)
 {
     count = 1;
     while (res==0 && count<5)
   {
    res = SD_init();
     count++;    
   }
   if (res==1)
   {
    FTDI.println(F("CARD INITIALIZED"));
    break;
   }
   else
   {
    FTDI.println(F("CARD FAILED"));
    delay(1000);
    res=SD_init();
   }
 }
    // Set up ESP for operation
     esp_init();
  //Check for configuration data 
  
  if (EEPROM.read(0)!=1)
  {
    esp_init();
    FTDI.println(F("P-ESP"));
    server_init();
    FTDI.println(F("P-ESERV"));
    config_mode();
    FTDI.println(reply);
    save_config();
    EEPROM.write(0,1);
    esp_init();
  }
  retrieve_config();
  delay(1000);
  esp_cwmode();  
  sim_check();
}


void loop() {
  // put your main code here, to run repeatedly:
unsigned long previous_time = millis();
while(1)
{
       unsigned long current_time = millis();
       int subscibe_mode = 0;
       sim_debug(); 
       esp_debug();
       priority();                 
      
      if ((unsigned long)(current_time - previous_time) >= 20000)
    {              
      //Get system data from PC and Control and relay it to the server
      if (power_b()==1 && (xmit==1 || e_xmit==1))
       {
        send_data(pc);
        delete_data(pc);
       }
       if (control_b()==1 && (e_xmit==1 || xmit==1))
       {
        send_data(ctrl);
        delete_data(ctrl);
       }       
         previous_time = millis(); 
         delay(1000);
         subscribe_mode(); //set  subscription mode
     }
   }     
}


/*_-------------------------------GENERAL FUNCTIONS----------------------------------*/
void first_boot_config()
{
  pinMode(config_but,INPUT_PULLUP);
  delay(10);
  if (digitalRead(config_but) == LOW)
  {
    delay(500);
    if (digitalRead(config_but) == LOW)
   {
        EEPROM.write(0,0);
        digitalWrite(led_esp,HIGH);
        digitalWrite(led_sim,HIGH);
        FTDI.print(F("RESET"));
        while(1){}
   }
  }
}

void config_mode()
{
//  Serial.listen();
  delay(100);
  
  FTDI.println(F("*****************************************"));
  FTDI.println(F("Connect to wifi network and send configuration data in this key value format:"));
  FTDI.println(F("\"KEY      \":\"VALUE\""));
  FTDI.println(F("\"SERIAL_NO\":\"xxxxxxxxx\""));
  FTDI.println(F("\"SERVER_URL\":\"xxxxxxxxx\""));
  FTDI.println(F("\"SERVER_PORT\":\"xxxxxxxxx\""));
  FTDI.println(F("\"SERVER_SSL_PORT\":\"xxxxxxxxx\""));
  FTDI.println(F("\"SERVER_USER\":\"xxxxxxxxx\""));
  FTDI.println(F("\"SERVER_PWD\":\"xxxxxxxxx\""));
  FTDI.println(F("\"SIM_APN\":\"xxxxxxxxx\""));
  FTDI.println(F("\"SIM_USER\":\"xxxxxxxxx\""));
  FTDI.println(F("\"SIM_PWD\":\"xxxxxxxxx\""));
  
  while(1)
  {
    led_blink(led_sim);led_blink(led_esp);
//    Serial.listen();
    if (Serial.find("SERIAL_NO=")){serial_no=Serial.readStringUntil('&'); break;}
  }
  digitalWrite(led_sim,false);
  long timer=millis();
  while((timer+1500)>millis())
  {
    if (Serial.find("SERVER_URL=")){server_url=Serial.readStringUntil('&'); break;}
  }
  timer=millis();
  while((timer+1500)>millis())
  {
    if (Serial.find("PORT=")){port=Serial.readStringUntil('&'); break;}
  }
  timer=millis();
  while((timer+1500)>millis())
  {
    if (Serial.find("SSL_PORT=")){ssl_port=Serial.readStringUntil('&'); break;}
  }
  timer=millis();
  while((timer+1500)>millis())
  {
    if (Serial.find("SERVER_USER=")){username=Serial.readStringUntil('&'); break;}
  }
  timer=millis();
  while((timer+1500)>millis())
  {
    if (Serial.find("SERVER_PWD=")){password=Serial.readStringUntil('&'); break;}
  }
  while((timer+1500)>millis())
  {
    if (Serial.find("SIM_APN=")){sim_apn=Serial.readStringUntil('&'); break;}
  }
  while((timer+1500)>millis())
  {
    if (Serial.find("SIM_USER=")){sim_user=Serial.readStringUntil('&'); break;}
  }
  while((timer+1500)>millis())
  {
    if (Serial.find("SIM_PWD=")){sim_pwd=Serial.readStringUntil('\n'); break;}
  }
  FTDI.println(serial_no);
  FTDI.println(server_url);
  FTDI.println(port);
  FTDI.println(ssl_port);
  FTDI.println(username);
  FTDI.println(password);
  FTDI.println(sim_apn);
  FTDI.println(sim_user);
  FTDI.println(sim_pwd);
  led_blink(led_esp);
}

  
  void priority()
{
  if (e_xmit==1)
  {
    _priority=0;    //ESP8266 is set as top priority when its available    
   esp_mqtt_ping();     //ping server to see if any data is available
  }
  else 
  {
    _priority=1;   //SIM is set as priority only when esp is not available
    mqtt_ping();     //ping server to see if any data is available
  }
}

short receive_data(char portx)  //Checks to receive data from a serial port using flag. Returns 1 if data was received, returns 0 if not. 
{
  unsigned short x=0;
  char data;
  switch (portx)
  {
    case 'p':  //get PC data
        {  
          reply="Z";
          power.listen();
          while (power.available()>0) 
          {            
            data=power.read();
            reply+=data;
            FTDI.print(data);
            x=1;  
            delay(5);
          }
          return x;
          break;
          }
     case 's':
         {
         reply="";
          SIM.listen();
          while (SIM.available()>0) 
          {            
            data=SIM.read();
            reply+=data;
            x=1;  
            delay(5);
          }
          return x;
          break;
        }
  case 'k':    //get sim data for subscribe
         {
         reply="";
          SIM.listen();
          if (SIM.find("s/ESPDATA")) 
          {            
            reply+=SIM.readStringUntil('\x65');//FTDI.println("data available");FTDI.println(reply);
            if(reply.indexOf('1')>=0)
            {
              control.listen();
              control.write(0x73);
              message="received";
              log_data(ctrl,message);
              send_data(ctrl);
              delete_data(ctrl);
            }
            x=1;  
          delay(5);
          }
          return x;
          break;
        }      
  case 'q':   //get esp data for subscribe
         {
         reply="";
          //Serial.listen();
          if (Serial.find("s/ESPDATA")) 
          {            
            reply+=Serial.readStringUntil('\x65');//FTDI.println("data available");
            if(reply.indexOf('1')>=0)
            {
              control.listen();
              control.write(0x73);
              message="received";
              log_data(ctrl,message);
              send_data(ctrl);
              delete_data(ctrl);
            }
            x=1;  
            delay(5);
          }
          return x;
          break;
        }
     case 'e':{
            reply="";
            //Serial.listen();
            while (Serial.available()>0) 
            {              
              data=Serial.read();
              reply+=data;
              x=1;  
              delay(5);
            }
            return x; break;
        }
    case 'c':{
      reply="A";
      control.listen();
      while (control.available()>0) 
      {        
        data=control.read();
        reply+=data;
        FTDI.print(data);
        x=1;  
        delay(5);
      }
      return x;break;
      }
      default: break;
    }
}
//----------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
void led_blink(int led)
{
  digitalWrite(led,HIGH);
  delay(100);
  digitalWrite(led,LOW);
}

void subscribe_mode()
   {     
     if (xmit == 1)
     {      
      mqtt_subscribe(); //subscribe with sim module if it is the available option from set of priorities
      long a = millis();
          while (millis()-a <= 5000) //wait for 8seconds to receive any incoming data 
          {
            receive_data('k');            
          }
         }
     else if ( e_xmit == 1)
     {
       esp_mqtt_subscribe();//subscribe with esp(wifi) module if it is the available option from set of priorities
        long a = millis(); 
        //wait for 8seconds to receive any incoming data 
          while (millis()-a <= 5000)
          {
            receive_data('q');            
          }                  
     }
        priority();    // set module priority
    }

/*********************************************************SD CARD FUNCTIONS************************************************************/
int SD_init()
{
  if (!SD.begin(4)) {return 0;}else { return 1;}
}

/*------------------------------------------------CONTROL AND PC FUNCTION-----------------------------------*/
short power_b()
{
  short retry=0; time_stamp = "";message ="";
  power.listen();
  while(retry<2)
  {
    power.write(0x64);
    delay(1000);
    if (receive_data('p')==1)
    {      
      time_stamp = get_currentTime();
      reply.remove(reply.indexOf('\n'));
      message = reply+"-"+time_stamp;
      //message.remove(message.indexOf(";"));
      log_data(pc,message);
      return 1;
    }
     retry++;
  }  
  return 0;
}

short control_b()
{
  short retry=0;time_stamp = "";message ="";
  control.listen();
  while(retry<2)
  {
    control.write(0x65);
    delay(500);
    if (receive_data('c')==1)
    {
      time_stamp = get_currentTime();
      reply.remove(reply.indexOf("\n"));
      message = reply+"-"+time_stamp;
      //message.remove(message.indexOf(";"));
      log_data(ctrl,message);
      return 1;
    }
     retry++;
  }  
  return 0;
}

/*************************************ESP FUNCTION**********************************************************/
void esp_init()
{ 
  //Serial.listen();
  delay(1); 
 while (1)
  {
    //Serial.println("AT");
    Serial.write(get_Rom_Data(esp_commands,e_check));Serial.write("\r\n");
    delay(500);
    if (Serial.find("OK"))
    {
      led_blink(led_esp);
      //Serial.println("AT+RST");
      Serial.write(get_Rom_Data(esp_commands,e_reset));Serial.write("\r\n");
      delay(500);
      if(Serial.find("OK") )
      {
        led_blink(led_esp);
       break;
      }
      else {digitalWrite(led_esp,HIGH);}
    }
    FTDI.println(Serial.readString());
    FTDI.println(F("NR"));   //No response
  } 
}

void server_init()
{ 
  //Serial.listen();
  delay(100);      
  //Serial.println("AT+CWQAP");   //disconnect any existing connection
  Serial.write(get_Rom_Data(esp_commands,e_disconnect));Serial.write("\r\n");
  if(!Serial.find("OK"))
  {
    digitalWrite(led_esp,HIGH);  //indicates an error
  }
   while(1)
   { 
      led_blink(led_esp);
      //Serial.println("AT+CWMODE=2");   //set up AP mode
      Serial.write(get_Rom_Data(esp_commands,e_mode));Serial.write("=3\r\n");
      delay(600);
      if(!Serial.find("OK"))
      {
        digitalWrite(led_esp,HIGH);  //indicates an error
        return;
      }
       led_blink(led_esp);
       //Serial.println("AT+CWSAP=\""+ssid_esp+"\",\""+password_esp+"\",5,0,4");  //configure SSID and password
       Serial.write(get_Rom_Data(esp_commands,e_config));Serial.write('=');Serial.write('"');Serial.write(get_Rom_Data(esp_data,ssid));
       Serial.write('"');Serial.write(',');Serial.write('"');Serial.write(get_Rom_Data(esp_data,passwrd));Serial.write('"');Serial.write(',');
       Serial.write("5,0,4\r\n");
       delay(600);
       
       if(!Serial.find("OK"))
       {
         digitalWrite(led_esp,HIGH);  //indicates an error
         return;
       }
       led_blink(led_esp);
     //Serial.println("AT+CIPAP=\""+HTTP_Ip+"\"");  //Assign MAC address to esp
      Serial.write(get_Rom_Data(esp_commands,e_ip));Serial.write('=');Serial.write('"');Serial.write(get_Rom_Data(esp_data,ip));Serial.write('"');
      Serial.write("\r\n");
      
      delay(100);
      if(!Serial.find("OK"))
      {
       digitalWrite(led_esp,HIGH);  //indicates an error
       return;
      }
      led_blink(led_esp);
     // Serial.println("AT+CIPMUX=1");  //Set to allow multiple connections
      Serial.write(get_Rom_Data(esp_commands,e_type));Serial.write(0x0D);Serial.write(0x0A);
      delay(100);
              
      if(!Serial.find("OK"))
      {
       digitalWrite(led_esp,HIGH);  //indicates an error
       return;
      }
      led_blink(led_esp);
      //Serial.println("AT+CIPSERVER=1,80");  //creates TCP server
      Serial.write(get_Rom_Data(esp_commands,e_server));Serial.write(0x0D);Serial.write(0x0A);
      delay(600);
      if(!Serial.find("OK"))
      {
        digitalWrite(led_esp,HIGH);  //indicates an error
        return;
      }
       led_blink(led_esp);
       delay(250);
       return;
     }
}

void esp_cwmode()
{
  //Serial.listen();
  delay(10);  
  Serial.write(get_Rom_Data(esp_commands,e_mode));Serial.write("=1\r\n");
  delay(50);  
  receive_data('e');
  led_blink(led_esp);
}
void esp_status()
{
 // Serial.listen();
  delay(10);
  Serial.write(get_Rom_Data(esp_commands,e_status));Serial.write("\r\n");
  delay(300);
  receive_data('e');
  FTDI.println(reply);

  if (check_response("STATUS:2",reply)==1)
  {
    _esp_status=2;
  }
  else if (check_response("STATUS:3",reply)==1)
  {
    _esp_status=3;
  }
  else if (check_response("STATUS:4",reply)==1)
  {
    _esp_status=4;
  }
  else if (check_response("STATUS:5",reply)==1)
  {
    _esp_status=5;
  }
}
//-----------------------------------------------------------------------------------------
void esp_debug()
{
  //Serial.listen();
  delay(10);
 
  esp_status();
   
  if (_esp_status==2)  // Status: Connected to AP. Start TCP
  {
    e_xmit=0;
    esp_cipstart();
    esp_status();
    digitalWrite(led_esp,HIGH);
  }
  if (_esp_status==3)  // Status: TCP connection established. Ping server
  {
   if (e_xmit==0)
   {
     esp_mqtt_connect();
     e_xmit=1;
   }
   led_blink(led_esp);
  }
  if (_esp_status==4)  // Status: TCP transmission disconnected. Restart TCP connection
  {
    e_xmit=0;
    esp_cipstart();
    digitalWrite(led_esp,HIGH);
  }
  if (_esp_status==5)  // Status: Not connected to AP. Join AP
  {
    esp_joinap();
    digitalWrite(led_esp,HIGH);    
  }  
}
//-----------------------------------------------------------------------------------------
void esp_cipstart()
{
  //Serial.listen();
  delay(10);
  
  //Serial.print("AT+CIPSTART=\"TCP\",\""+server_url+"\","+"\""+port+"\"""\r\n");
  Serial.write(get_Rom_Data(esp_commands,e_start)); Serial.write('"');Serial.print(server_url);
  Serial.write('"');Serial.write(','); Serial.print(port); Serial.write("\r\n");
  delay(50);
  
  receive_data('e');
  FTDI.println(reply);
  delay(100);
  
  led_blink(led_esp);
}
//------------------------------------------------------------------------------------------
void esp_joinap()
{
  //Serial.listen();
  delay(10);
  
  Serial.write(get_Rom_Data(esp_commands,e_join));Serial.write('=');Serial.write('"');Serial.write(get_Rom_Data(esp_data,hotspot)); Serial.write('"');Serial.write(',');
  Serial.write('"');Serial.write(get_Rom_Data(esp_data,hotspot_pass)); Serial.write('"');Serial.write("\r\n");
  delay(100);
  receive_data('e');
  FTDI.println(reply);
  led_blink(led_esp);
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
void esp_mqtt_connect()
{
  int retry = 0;
const char protocol[10]="MQIsdp", protocol_lvl=0X03, flag=0xC2;
unsigned int keep_alive=60;
unsigned char  encodedByte;
unsigned short protocol_len, clid_len,username_len, pw_len;          

  //Serial.listen();
  delay(10);  
  
  protocol_len=strlen(protocol);
  clid_len=serial_no.length();
  username_len=username.length();
  pw_len=password.length();
  data_len=protocol_len+clid_len+username_len+pw_len+12;
  unsigned long x=data_len;
  short len=data_len+2;

  //Serial.print("\r\nAT+CIPSEND\r\n");
  Serial.write(get_Rom_Data(esp_commands,e_send));Serial.write('=');
  Serial.print(len);Serial.write("\r\n");
  
  delay(50);
  Serial.write(0x10);     //control byte for mqtt connection
  
  receive_data('e');
  FTDI.println(reply);
  do
  {
    encodedByte = x%128;
     x = x/128;
    if ( x > 0 ){encodedByte |= 128;}
    Serial.write(encodedByte);
  }while ( x > 0 );

  Serial.write(protocol_len>>8);
  Serial.write(protocol_len & 0xFF);
  Serial.print(protocol);

  Serial.write(protocol_lvl);
  Serial.write(flag);
  Serial.write(keep_alive>>8);
  Serial.write(keep_alive & 0xFF);

  Serial.write(clid_len>>8);
  Serial.write(clid_len & 0xFF);
  Serial.print(serial_no);

  Serial.write(username_len>>8);
  Serial.write(username_len & 0xFF);
  Serial.print(username);

  Serial.write(pw_len>>8);
  Serial.write(pw_len & 0xFF);
  Serial.print(password);

  delay(2000);
  
  receive_data('e');
  FTDI.println(reply);
 
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
void esp_mqtt_publish(String msg)
{
  String topic="d/"+serial_no;
  topic.trim();
  unsigned short topic_len,message_len;
  unsigned char  encodedByte;
  
  topic_len=topic.length();
  message_len=msg.length();
  data_len=message_len+topic_len+2;
  short len =data_len+2;

  //Serial.listen();
  delay(10);
  
  Serial.write(get_Rom_Data(esp_commands,e_send));Serial.write('=');Serial.print(len);
  Serial.write(0x0D);Serial.write(0x0A);
  delay(50);

  Serial.write(0x30);     //control byte for mqtt connection
  unsigned long x=data_len;
  do
  {
    encodedByte = x%128;
     x = x/128;
    if ( x > 0 ){encodedByte |= 128;}
    Serial.write(encodedByte);
    
  }while ( x > 0 );

  Serial.write(topic_len>>8);
  Serial.write(topic_len & 0xFF);
  Serial.print(topic);
  
  Serial.print(msg);
  receive_data('e');
  delay(100);
}
//------------------------------------------------------------------------------------------
void esp_mqtt_ping()
{
  char ping=0x00;

 // Serial.listen();
  delay(10);
  Serial.write(get_Rom_Data(esp_commands,e_send));Serial.write('=');Serial.print('2');
  Serial.write(0x0D);Serial.write(0x0A);
  
  delay(50);
  
  Serial.write(0xC0);

  Serial.write(ping);

  receive_data('q');
  
}
void esp_mqtt_subscribe()
{
  String topic="s/"+serial_no;
  topic.trim();
  uint8_t topic_len,message_len, QoS=0x00,packet_id=0x01;
  unsigned char  encodedByte;  

  topic_len=topic.length();//strlen(topic);
  data_len=topic_len+5;
  short len=data_len+2;
//    Serial.listen();    
    delay(10);
    
    Serial.write(get_Rom_Data(esp_commands,e_send));Serial.write('=');Serial.print(len);Serial.write(0x0D);Serial.write(0x0A);
    delay(100);  
    Serial.write(0x82); 
  receive_data('e');
  FTDI.println(reply);      
  unsigned long x=data_len;
  do
  {
    encodedByte = x%128;
     x = x/128;
    // if there are more data to encode, set the top bit of this byte
    if ( x > 0 ){encodedByte |= 128;}
    Serial.write(encodedByte);
    
  }while ( x > 0 );

  Serial.write(packet_id>>8);
  packet_id=0x01;
  Serial.write(packet_id&0xFF);
  
  Serial.write(topic_len>>8);
  Serial.write(topic_len & 0xFF);
  Serial.print(topic);
  
  Serial.write(QoS);  //QoS
 delay(10);
 Serial.write(0x1A);
  receive_data('q');
}

/***********************************************SIM FUNCTIONS **********************************/

short sim_check()
{
  SIM.listen();
  delay(10);
    //SIM.print("AT\r\n");
    SIM.write(get_Rom_Data(sim_commands,s_check));SIM.write("\r\n");
    delay(50);
    receive_data('s');    
    if (check_response("OK",reply)==0)
    {
      FTDI.println("f-SIM"); 
      return 0;
    }
    else{FTDI.println("P-SIM");return 1;}
    led_blink(led_sim);
}

void sim_status()
{
  SIM.listen();
  delay(10);
  
    SIM.write(get_Rom_Data(sim_commands,s_status));SIM.write("\r\n");
    delay(200);
    receive_data('s');
    FTDI.println(reply);    
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
  SIM.listen();
  delay(10);
   
  sim_status();
  
  if (_sim_status==0)  // status:IP INITIAL. Shut IP. Set APN. 
  {
    sim_cipshut();
    sim_set_apn();
    sim_status();
    digitalWrite(led_sim,HIGH);
  }
  if (_sim_status==1)  // status:IP START. Activate GPRS connection
  {
    xmit=0;
    sim_gprs_act();
    sim_status();
    digitalWrite(led_sim,HIGH);
  }
  if (_sim_status==3)  // status:IP GPRSACT. Get IP 
  {
    xmit=0;
    sim_ip();
    sim_status();
    digitalWrite(led_sim,HIGH);
  }
  if (_sim_status==4)  // status:IP STATUS. Start TCP connection
  {
    xmit=0;
    sim_cipssl();
    sim_cipstart();
    sim_status();
    digitalWrite(led_sim,HIGH);
  }
  if (_sim_status==5)  // status:TCP CONNECTING. Wait for TCP connection
  {
    xmit=0;
    digitalWrite(led_sim,HIGH);
  }
  if (_sim_status==6)  // status:CONNECT OK. Set transmit flag 
  {
    if (xmit==0)
    {
      mqtt_connect();
      xmit=1;
     }
    led_blink(led_sim);
  }
  if (_sim_status==7)  // status:TCP CLOSING. Clear transmit flag. 
  {
    xmit=0;
    sim_status();
    digitalWrite(led_sim,HIGH);
  }
  if (_sim_status==8)  // status:TCP CLOSED. Restart TCP connection. 
  {
    xmit=0;
    sim_cipstart();
    digitalWrite(led_sim,HIGH);
  }
  if (_sim_status==9)  // status:PDP DEACT. Shut IP.
  {
    xmit=0;
    sim_cipshut();
    digitalWrite(led_sim,HIGH);
  }
  
}

void sim_cipshut()
{
  SIM.listen();
  delay(10);
  //AT+CIPSHUT
  SIM.write(get_Rom_Data(sim_commands,s_shut));SIM.write(0x0D);SIM.write(0x0A);
  delay(50);
  receive_data('s');  
  FTDI.println(reply);
  led_blink(led_sim);
}
void sim_set_apn()
{ 
  SIM.listen();
  delay(10); 
  //SIM.print("AT+CSTT=\"www\",\"\",\"\"\r\n");    //set apn
  SIM.write(get_Rom_Data(sim_commands,s_apn));SIM.write('=');SIM.write('"');SIM.print(sim_apn);SIM.write('"');
  SIM.write(',');SIM.write('"');SIM.write('"');SIM.write(',');SIM.write('"');
  SIM.write('"');SIM.write("\r\n");
  delay(50);
  receive_data('s');
  FTDI.println(reply);
  led_blink(led_sim);
    
}
//------------------------------------------------------------------------------------------
void sim_gprs_act()
{
    SIM.listen();
    delay(10);
    
    //AT+CIICR
    SIM.write(get_Rom_Data(sim_commands,s_gprs));SIM.write("\r\n");
    delay(50);
    receive_data('s');
    FTDI.println(reply);
   led_blink(led_sim);
}
//---------------------------------------------------------------------------------------
void sim_ip()
{
  SIM.listen();
  delay(10);

  //AT+CIFSR
  SIM.write(get_Rom_Data(sim_commands,s_config));SIM.write(0x0D);SIM.write(0x0A);
  delay(50);  
  receive_data('s');
  FTDI.println(reply);
  led_blink(led_sim);
}
//---------------------------------------------------------------------------------------
void sim_cipssl()
{
  SIM.listen();
  delay(10);
  //AT+CIPSSL=1
  SIM.write(get_Rom_Data(sim_commands,s_ssl));SIM.write('=');SIM.write('0');
  SIM.write(0x0D);SIM.write(0x0A);
  delay(50);
  receive_data('s');
  FTDI.println(reply);
 led_blink(led_sim);
}
//----------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
void sim_cipstart()
{  
  SIM.listen();
  delay(10);  
  //SIM.print("AT+CIPSTART=\"TCP\",\""+server_url+"\","+"\""+port+"\"""\r\n");
  SIM.write(get_Rom_Data(sim_commands,s_start)); SIM.write('"');SIM.print(server_url);
  SIM.write('"');SIM.write(','); SIM.write('"');SIM.print(port);
  SIM.write('"');SIM.write("\r\n");
  delay(50);  
  receive_data('s');
  FTDI.println(reply); 
  led_blink(led_sim);  
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//pasword :RBNegtlTDhLH  username:nxbdggts  port:12193 host:m13.cloudmqtt.com
//'wwrivypi',
  //  password: 'C6r-EMOfyxb4'
int mqtt_connect()
{
const char protocol[10]="MQIsdp", protocol_lvl=0X03, flag=0xC2;
unsigned int keep_alive=60;
unsigned char  encodedByte;
unsigned short protocol_len, clid_len,username_len, pw_len;  
String client_id="sim"+serial_no;       

  SIM.listen();
  delay(10);
  
  //SIM.print("\r\nAT+CIPSEND\r\n");
  SIM.write(get_Rom_Data(sim_commands,s_send));SIM.write("\r\n");
  delay(50);
  receive_data('s');
  FTDI.println(reply);
  SIM.write(0x10);     //control byte for mqtt connection
  protocol_len=strlen(protocol);
  clid_len=client_id.length();
  username_len=username.length();
  pw_len=password.length();
  data_len=protocol_len+clid_len+username_len+pw_len+12;
  unsigned long x=data_len;
  do
  {
    encodedByte = x%128;
     x = x/128;
    if ( x > 0 ){encodedByte |= 128;}
    SIM.write(encodedByte);
    //FTDI.print(encodedByte);
  }while ( x > 0 );

  SIM.write(protocol_len>>8);
  SIM.write(protocol_len & 0xFF);
  SIM.print(protocol);

  SIM.write(protocol_lvl);
  SIM.write(flag);
  SIM.write(keep_alive>>8);
  SIM.write(keep_alive & 0xFF);

  SIM.write(clid_len>>8);
  SIM.write(clid_len & 0xFF);
  SIM.print(client_id);

  SIM.write(username_len>>8);
  SIM.write(username_len & 0xFF);
  SIM.print(username);

  SIM.write(pw_len>>8);
  SIM.write(pw_len & 0xFF);
  SIM.print(password);
  SIM.write(0x1A);
  delay(1000);
  receive_data('s');
  FTDI.println(reply);

}
//---------------------------------------------------------------------------------
short mqtt_publish(String msg)//, String topic)
{
  String topic="d/"+serial_no;
  topic.trim();
  unsigned short topic_len,message_len;
  unsigned char  encodedByte;  
    SIM.listen();
    delay(10);
    SIM.write(get_Rom_Data(sim_commands,s_send));SIM.write("\r\n");
    delay(50);
  
  SIM.write(0x30);     //control byte for mqtt connection
  topic_len=topic.length();//strlen(topic);
  message_len=msg.length();
  data_len=message_len+topic_len+2;

  unsigned long x=data_len;
  do
  {
    encodedByte = x%128;
     x = x/128;
    // if there are more data to encode, set the top bit of this byte
    if ( x > 0 ){encodedByte |= 128;}
    SIM.write(encodedByte);
    
  }while ( x > 0 );

  SIM.write(topic_len>>8);
  SIM.write(topic_len & 0xFF);
  SIM.print(topic);
  
  SIM.print(msg);

  SIM.write(0x1A);
  delay(10);
  receive_data('s');
  
}
//---------------------------------------------------------------------------------
void mqtt_ping()
{
  
   char disconn=0x00;

  SIM.listen();
  delay(10);
  SIM.write(get_Rom_Data(sim_commands,s_send));SIM.write("\r\n");
  delay(50);
  
  SIM.write(0xC0);

  SIM.write(disconn);
  
  SIM.write(0x1A);
  
  delay(500);

  receive_data('k');
}

//-----------------------------------------------------------------------------------------
void mqtt_subscribe()
{
  String topic="s/"+serial_no;
  topic.trim();
  uint8_t topic_len,message_len, QoS=0x00,packet_id=0x01;
  unsigned char  encodedByte;  

  topic_len=topic.length();//strlen(topic);
  data_len=topic_len+5;
  short len = data_len+2;
  
    SIM.listen();
    
    delay(10);
    
    SIM.write(get_Rom_Data(sim_commands,s_send));SIM.write("\r\n");
    delay(50);  
  SIM.write(0x82);
  unsigned long x=data_len;
  do
  {
    encodedByte = x%128;
     x = x/128;
    // if there are more data to encode, set the top bit of this byte
    if ( x > 0 ){encodedByte |= 128;}
    SIM.write(encodedByte);
    
  }while ( x > 0 );

  SIM.write(packet_id >> 8);
  SIM.write(packet_id & 0xFF);
  
  SIM.write(topic_len >> 8);
  SIM.write(topic_len & 0xFF);
  SIM.print(topic);
  
  SIM.write(QoS);  //QoS

  SIM.write(0x1A);
  delay(100);
  receive_data('k');
    
}


// READING and WRITING to SD card
void save_config()
{
  delete_data("CONFIG");
  delay(50);
  log_data("CONFIG",serial_no);
  log_data("CONFIG",server_url);
  log_data("CONFIG",port);
  log_data("CONFIG",ssl_port);
  log_data("CONFIG",username);
  log_data("CONFIG",password);
  log_data("CONFIG",sim_apn);
  log_data("CONFIG",sim_user);
  log_data("CONFIG",sim_pwd);
}
//-----------------------------------------------------------------------------------------
short retrieve_config()
{
  if (verify_data("CONFIG")==1)
  {
    led_blink(led_esp);
    readData("CONFIG");
    return 1; //file not found
  }           
  digitalWrite(led_esp,LOW);
  return 0;   //configuration data retrieved successfully
}
short log_data(String filename,String msg)
{
  file = filename + ".TXT";
  File dataFile;
  dataFile = SD.open(file, FILE_WRITE);
   if (dataFile)
   {
    dataFile.println(msg);
    dataFile.close();
    FTDI.println(msg+" written to "+file);
    return 1;
   }
   else
   {
    FTDI.println(F("ERROR OCCURED DURING WRITING FILE"));
   return 0;
   }
}
void delete_data(String filename)
{
  file = filename + ".TXT";
  if (SD.exists(file))
      {
        SD.remove(file);
        delay(100);
        if (!SD.exists(file))
          {
            led_blink(led_sd); 
            FTDI.println(F("FILE DELETED"));
            }
        else {FTDI.println(F("(DELETE ERROR"));digitalWrite(led_sd,HIGH);}
      }
  else 
      {
        FTDI.println("FILE DOES NOT EXIST");
        digitalWrite(led_sd,HIGH);   
      }
}

short verify_data(String filename)
{
    file = filename+".TXT";  
    if (SD.exists(file))
      {
        FTDI.print("FILE EXISTS");
        return 1;
      }
      else 
      {
        FTDI.println("FILE DOES NOT EXIST");
        digitalWrite(led_sd,HIGH);
        return 0;
      }        
}

short send_data(String filename)
{
  file = filename+".TXT";
  File dataFile;
      dataFile = SD.open(file);
      if (dataFile) 
      {  
        do 
        {
            message="";
            message=dataFile.readStringUntil('\n');
            if (_priority==0)
              {
                esp_mqtt_publish(message);
              }else if (_priority==1)
              {
                mqtt_publish(message);
              }
              FTDI.println("OK");
        }
        while (dataFile.peek() != -1)   ;  
        dataFile.close();   
      }     
  digitalWrite(led_sd,LOW);
}

void readData(String filename)
{
  file = filename+ ".TXT"; 
  File dataFile;
      dataFile = SD.open(file);
      if (dataFile) 
      {  
        int i = 0;
          do 
        {
            message="";
            message=dataFile.readStringUntil('\n');
            message.trim();
            config_data[i] =message; 
            i++;
            if (i==9) {break;}
            FTDI.print("OK");
            delay(50);
        }
        while (dataFile.peek() != -1)   ;  
        dataFile.close();   
      
       serial_no = config_data[0];
       FTDI.println("Serial_no:"+serial_no);
       server_url= config_data[1];
       FTDI.println("Server URL:"+server_url);
       port=config_data[2];
       FTDI.println("Port:"+port);
       ssl_port=config_data[3];
       FTDI.println("SSL port:"+ssl_port);
       username=config_data[4];
       FTDI.println("Username:"+username);
       password=config_data[5];
       FTDI.println("Password:"+password);
       sim_apn=config_data[6];
       FTDI.println("Sim APN:"+sim_apn);
       sim_user=config_data[7];
       FTDI.println("Sim User:"+sim_user);
       sim_pwd=config_data[8];
       FTDI.println("Sim Password:"+sim_pwd);
       }
       else
       {
        FTDI.println(F("FILE DOES NOT EXIST"));
        FTDI.println(F("YOU NEED TO CONFIGURE YOUR SYSTEM"));       
      }
}

//rtc code
String get_currentTime()
{
   DateTime now = rtc.now();
   
   int a =now.day();int b =now.month();int c =now.year();
    int d =now.hour();int e =now.minute();int f =now.second();
    String local_time = String(a) + "/" +String(b) + "/" +String(c)+","+String(d) + "/" +String(e) + "/" +String(f);
    delay(50);
    return local_time;
}
