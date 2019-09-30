#include <SoftwareSerial.h>

String sim_apn = "web.gprs.mtnnigeria.net", sim_pswd = "", sim_user = "", location,result;
//cloud mqtt address to monitor the data; Username: akeemolalekan229@gmail.com : password: photizzo : instance: Espdata
String serial_no = "GPS_data", server_ip ="52.3.184.147",  server_url = "m12.cloudmqtt.com", server_port = "13065", server_user = "rupoogkd", server_pswd ="fswD8ouhN9E_", server_ssl = "23065";


SoftwareSerial gps(10,11);
SoftwareSerial sim(9,8);

void check_response()
{
  char data; result = "";
  
  while(sim.available()>0)
  {         
          data = sim.read();         
          result += data;
          delay(5);                
  }
  Serial.println(result);
// if (sim.find("+IP ERROR: Network n"))
//  {
//    establishGPRS_conn(); //establishing GPRS connection 
//      delay(100);
//     openTCP_connection(); //open TCP for Data communication
//     delay(200);
//  }

}
int mqtt_connect()
{
  
  sim.listen();
  delay(10);
  
  const char protocol[10]="MQIsdp", protocol_lvl=0X03, flag=0xC2;
  unsigned int keep_alive=60;
  unsigned char  encodedByte;
  unsigned short protocol_len, clid_len,username_len, pw_len,data_len;  
  String client_id="sim"+serial_no; 

  sim.listen();
  delay(10);
     
  sim.write(0x10);     //control byte for mqtt connection
  protocol_len=strlen(protocol);
  clid_len=serial_no.length();
  username_len=server_user.length();
  pw_len=server_pswd.length();
  data_len=protocol_len+clid_len+username_len+pw_len+12;
   short len =data_len+2;
   
    sim.write("AT+CIPSEND=0,");
     sim.print(len);sim.write("\r\n"); 
     delay(50);
     sim.write(0x10); 

  unsigned long x=data_len;
  do
  {
    encodedByte = x%128;
     x = x/128;
    if ( x > 0 ){encodedByte |= 128;}
    sim.write(encodedByte);
    //Serial.print(encodedByte);
  }while ( x > 0 );

  sim.write(protocol_len>>8);
  sim.write(protocol_len & 0xFF);
  sim.print(protocol);

  sim.write(protocol_lvl);
  sim.write(flag);
  sim.write(keep_alive>>8);
  sim.write(keep_alive & 0xFF);

  sim.write(clid_len>>8);
  sim.write(clid_len & 0xFF);
  sim.print(serial_no);

  sim.write(username_len>>8);
  sim.write(username_len & 0xFF);
  sim.print(server_user);

  sim.write(pw_len>>8);
  sim.write(pw_len & 0xFF);
  sim.print(server_pswd);
  //sim.write(0x1A);
  delay(500);
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
  
    sim.listen();
    delay(10);
    
     sim.write("AT+CIPSEND=0,");
     sim.print(len);sim.write("\r\n"); 
     delay(50);
  
  sim.write(0x30);     //control byte for mqtt connection
  
  unsigned long x=data_len;
  do
  {
    encodedByte = x%128;
     x = x/128;
    // if there are more data to encode, set the top bit of this byte
    if ( x > 0 ){encodedByte |= 128;}
    sim.write(encodedByte);
    
  }while ( x > 0 );

  sim.write(topic_len>>8);
  sim.write(topic_len & 0xFF);
  sim.print(topic);  
  sim.print(msg);
// sim.write(0x1A);
  delay(100);
check_response();
delay(100); 
//sim.write("AT+CIPCLOSE=0\r\n"); 
//delay(100);
}
//---------------------------------------------------------------------------------

void mqtt_ping()
{
  
   char disconn=0x00;

  sim.listen();
  delay(10);
  sim.write("AT+CIPSEND=0,2\r\n"); 
  delay(50);
  
  sim.write(0xC0);

  sim.write(disconn);  
  //sim.write(0x1A);  
  delay(500);
  check_response();
}

String GPSdata(String k)
{  
  String a[10] = {"","","","","","","","","",""},val = "",answer =" ";int m=0;
   for (int i = 0; i< k.length(); i++)
{
  if(k.charAt(i)==(','))
    {     
     a[m] = val;
     m++;
    val = "";
    }
    else
    {
     val += k[i]; 
    }
}
    String times = a[0];
    String date = a[8];
    String gpsstatus = a[1];
    if (k.indexOf('V') >= 1)
      {        
         answer = "0000,000,000,000,000";
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
      return answer;       
}
void openTCP_connection()
{
  //establishGPRS_conn();
  sim.write("AT+CIPOPEN=0,\"TCP\",\"");
  sim.print(server_ip); 
  sim.write("\","); 
  sim.print(server_port);
  sim.write("\r\n"); 
  delay(100);
  check_response();  
}
void publish(String a)
{
      sim.listen();
      openTCP_connection();
      delay(100);
       mqtt_connect();
       delay(100);
       mqtt_publish(a);   
       delay(100);
}
void simSetup()
{
  sim.listen();
  delay(50);
  sim.write("AT+CSQ\r\n");  
  delay(100);
  check_response();
  sim.write("AT+CPIN?\r\n");
  delay(100);   
  check_response();
  sim.write("AT+CREG?\r\n");
  delay(100);   
  check_response();
}

void establishGPRS_conn()
{
  sim.listen();
  delay(50);
  sim.write("AT+CGATT=1\r\n"); 
  delay(300);
  check_response();
  //PDP activation
 sim.write("AT+CGACT=1,1\r\n"); 
  delay(100);
  check_response();
  sim.write("AT+CGSOCKCONT=1,\"IP\",\"");
  sim.print(sim_apn); sim.write("\"\r\n"); 
  delay(1000);
  check_response();
  
  if (result.substring('E'))
  {
    sim.write("AT+CGDCONT=1,\"IP\",\"" );
    sim.print(sim_apn); sim.write("\"\r\n"); 
    delay(100);
  }
   
  sim.write("AT+CSOCKSETPN=1\r\n"); 
  delay(1000);
  check_response();
  sim.write("AT+CIPMODE=0\r\n"); //set connection mode
  delay(1000);
  check_response();
  sim.write("AT+NETOPEN=,,1\r\n");  // open network for connection
  delay(1000);
  check_response();
  sim.write("AT+IPADDR\r\n"); //get IP Address of the Module
  delay(100);   
  check_response();
}




