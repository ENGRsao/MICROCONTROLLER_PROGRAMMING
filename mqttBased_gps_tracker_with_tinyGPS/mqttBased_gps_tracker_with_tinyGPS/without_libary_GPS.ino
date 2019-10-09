#include <SoftwareSerial.h>


SoftwareSerial gps(8,10);
SoftwareSerial gpstracker(9,11);

int data_available;String answer="";int x = 0;int networkstatus=0;String data = "";String a[12];int m=0;String val= "";

String sim_apn = "web.gprs.mtnnigeria.net", sim_pswd = "", sim_user = "", location,result;
String serial_no = "GPS_data", server_ip ="52.3.184.147",  server_url = "m12.cloudmqtt.com", server_port = "13065", server_user = "rupoogkd", server_pswd ="fswD8ouhN9E_", server_ssl = "23065";

 long timeout = 20000;
void setup() {
   // put your setup code here, to run once:
      Serial.begin(4800); //baud rate
      gpstracker.begin(4800);
      gps.begin(9600);
      delay(200);
      gps.write("$PUBX,40,GGA,0,0,0,0,0,0*5A\r\n");
      delay(100);
      gps.write("$PUBX,40,GLL,0,0,0,0,0,0*5A\r\n");
      delay(100);
      gps.write("$PUBX,40,GSV,0,0,0,0,0,0*5A\r\n");
      delay(100);
      gps.write("$PUBX,40,GGA,0,0,0,0,0,0*5A\r\n");
      delay(100);
      gps.write("$PUBX,40,ZDA,0,0,0,0,0,0*5A\r\n");
      delay(100);
      gpstracker.listen();
      gpstracker.write("AT\r\n");
      delay(100);
      simSetup();  //settting up sim module
      delay(100);
      establishGPRS_conn(); //establishing GPRS connection 
      delay(100);
      openTCP(); //open TCP for Data communication
      delay(200);
      mqtt_connect(); //connect to mqqt server
      delay(3000); 
      gps.listen();      
}
 unsigned long previous_time = millis();
void loop() {
  // put your main code here, to run repeatedly:    
      //GPSdata();
      unsigned long current_time = millis();
      if ((unsigned long)(current_time - previous_time) >= timeout)
    {     
      String  data="";
      Serial.println("Read data");
        gps.listen();
       while (!(gps.find("$GPRMC,")))
      {
      }
         data += gps.readStringUntil('*');
        Serial.println(data);     
        GPSdata()  ;    
           publish(answer);      
           previous_time = millis();            
     }
}

void GPSdata()
{  
  m = 0;
   for (int i = 0; i< data.length(); i++)
{
  if(data.charAt(i)==(','))
  {     
     a[m] = val;
     m++;
    val = "";
  }
  else
  {
   val += data[i]; 
  }
}
String times = a[0];
String date = a[8];
String gpsstatus = a[1];
if (data.indexOf('V') > 1)
      {
        
        Serial.println("GPS Not Fixed Yet");
        
      } 
      else
      {
      String latitude = a[2]+a[3];
      String longitude = a[4]+a[5];
      String speed_v = String(a[6].toInt() * 0.51444);      
      answer = latitude+","+longitude+","+speed_v+","+times+","+date;
      Serial.println("GPS Fixed");
      Serial.println(answer);
      }        
}

void publish(String a)
{
      gpstracker.listen();
       mqtt_connect();
       delay(1000);
       mqtt_publish(a);   
       delay(100);
}
void simSetup()
{
  gpstracker.listen();
  delay(50);
  gpstracker.write("AT+CSQ\r\n");  
  delay(100);
  check_response();
  gpstracker.write("AT+CPIN?\r\n");
  delay(100);   
  check_response();
  gpstracker.write("AT+CREG?\r\n");
  delay(100);   
  check_response();
}

void establishGPRS_conn()
{
  gpstracker.listen();
  delay(50);
  gpstracker.write("AT+CGATT=1\r\n"); 
  delay(1000);
  check_response();
  //PDP activation
 gpstracker.write("AT+CGACT=1,1\r\n"); 
  delay(100);
  check_response();
  gpstracker.write("AT+CGSOCKCONT=1,\"IP\",\"");
  gpstracker.print(sim_apn); gpstracker.write("\"\r\n"); 
  delay(1000);
  check_response();
  
  if (result.substring('E'))
  {
    gpstracker.write("AT+CGDCONT=1,\"IP\",\"" );
    gpstracker.print(sim_apn); gpstracker.write("\"\r\n"); 
    delay(100);
  }
   
  gpstracker.write("AT+CSOCKSETPN=1\r\n"); 
  delay(1000);
  check_response();
  gpstracker.write("AT+CIPMODE=0\r\n"); //set connection mode
  delay(1000);
  check_response();
  gpstracker.write("AT+NETOPEN=,,1\r\n");  // open network for connection
  delay(1000);
  check_response();
  gpstracker.write("AT+IPADDR\r\n"); //get IP Address of the Module
  delay(100);   
  check_response();
}

void openTCP()
{
  establishGPRS_conn();
  gpstracker.write("AT+CIPOPEN=0,\"TCP\",\"");
  gpstracker.print(server_ip); 
  gpstracker.write("\","); 
  gpstracker.print(server_port);
  gpstracker.write("\r\n"); 
  delay(100);
  check_response();
  
}

int mqtt_connect()
{
  openTCP();
  delay(100);
  check_response();
  delay(100);
  
  const char protocol[10]="MQIsdp", protocol_lvl=0X03, flag=0xC2;
  unsigned int keep_alive=60;
  unsigned char  encodedByte;
  unsigned short protocol_len, clid_len,username_len, pw_len,data_len;  
  String client_id="sim"+serial_no; 

  gpstracker.listen();
  delay(10);
     
  gpstracker.write(0x10);     //control byte for mqtt connection
  protocol_len=strlen(protocol);
  clid_len=serial_no.length();
  username_len=server_user.length();
  pw_len=server_pswd.length();
  data_len=protocol_len+clid_len+username_len+pw_len+12;
   short len =data_len+2;
   
    gpstracker.write("AT+CIPSEND=0,");
     gpstracker.print(len);gpstracker.write("\r\n"); 
     delay(50);
     gpstracker.write(0x10); 

  unsigned long x=data_len;
  do
  {
    encodedByte = x%128;
     x = x/128;
    if ( x > 0 ){encodedByte |= 128;}
    gpstracker.write(encodedByte);
    //Serial.print(encodedByte);
  }while ( x > 0 );

  gpstracker.write(protocol_len>>8);
  gpstracker.write(protocol_len & 0xFF);
  gpstracker.print(protocol);

  gpstracker.write(protocol_lvl);
  gpstracker.write(flag);
  gpstracker.write(keep_alive>>8);
  gpstracker.write(keep_alive & 0xFF);

  gpstracker.write(clid_len>>8);
  gpstracker.write(clid_len & 0xFF);
  gpstracker.print(serial_no);

  gpstracker.write(username_len>>8);
  gpstracker.write(username_len & 0xFF);
  gpstracker.print(server_user);

  gpstracker.write(pw_len>>8);
  gpstracker.write(pw_len & 0xFF);
  gpstracker.print(server_pswd);
  //gpstracker.write(0x1A);
  delay(1000);
  check_response();
}

short mqtt_publish(String msg)//, String topic)
{
 
  String topic="d/"+serial_no;
  topic.trim();
  unsigned short topic_len,message_len,data_len;
  unsigned char  encodedByte;

  topic_len=topic.length();//strlen(topic);
  message_len=msg.length();
  data_len=message_len+topic_len+2;
  short len =data_len+2;
  
    gpstracker.listen();
    delay(10);
    
     gpstracker.write("AT+CIPSEND=0,");
     gpstracker.print(len);gpstracker.write("\r\n"); 
     delay(50);
  
  gpstracker.write(0x30);     //control byte for mqtt connection
  
  unsigned long x=data_len;
  do
  {
    encodedByte = x%128;
     x = x/128;
    // if there are more data to encode, set the top bit of this byte
    if ( x > 0 ){encodedByte |= 128;}
    gpstracker.write(encodedByte);
    
  }while ( x > 0 );

  gpstracker.write(topic_len>>8);
  gpstracker.write(topic_len & 0xFF);
  gpstracker.print(topic);  
  gpstracker.print(msg);
// gpstracker.write(0x1A);
  delay(100);
check_response(); 
gpstracker.write("AT+CIPCLOSE=0\r\n"); 
delay(1000);
}
//---------------------------------------------------------------------------------

void mqtt_ping()
{
  
   char disconn=0x00;

  gpstracker.listen();
  delay(10);
  gpstracker.write("AT+CIPSEND=0,2\r\n"); 
  delay(50);
  
  gpstracker.write(0xC0);

  gpstracker.write(disconn);
  
  //gpstracker.write(0x1A);
  
  delay(500);
  check_response();

}

void check_response()
{
  char data; result = "";
  
  while(gpstracker.available()>0)
  {         
          data = gpstracker.read();         
          result += data;
          delay(5);                
  }
  Serial.println(result);
 if (gpstracker.find("+IP ERROR: Network n"))
  {
    establishGPRS_conn(); //establishing GPRS connection 
      delay(100);
     openTCP(); //open TCP for Data communication
     delay(200);
  }

}

