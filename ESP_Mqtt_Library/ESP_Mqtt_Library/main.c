/*
 * ESP_Mqtt_Library.c
 *
 * Created: 7/30/2018 4:27:48 PM
 * Author : ENGR SAO
 */ 
	
#include <avr/io.h>
#include <util/delay.h>
#include <SoftwareSerial.h>
#include <string.h>
#include <avr/pgmspace.h>

#define F_CPU 16000000UL
#define OPEN "OPEN"
#define CLOSED "CLOSED"


SoftwareSerial esp(4,5);
	
  char buffer[15];
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

 enum {e_check,e_reset,e_server,e_mode,e_disconnect,e_type,e_ip,e_config,e_send,e_status,e_start,e_join};
 
  char* get_Rom_Data(const char* Table_Details[],int i)
  {
	  strcpy_P(buffer,(char*) pgm_read_word(&Table_Details[i]));
	  return buffer;
  }

  void initialize_esp()
  {
	  esp.listen();
	  esp.print_string(get_Rom_Data(esp_commands,e_check));esp.print_string("\r\n");
  }
  
  void reset_esp()
  {
	  esp.print_string(get_Rom_Data(esp_commands,e_reset));esp.print_string("\r\n");
  }

  void disconnect_esp()
  {
	  esp.print_string(get_Rom_Data(esp_commands,e_disconnect));esp.print_string("\r\n");	  
  }
  
  void set_esp_mode(int a)
  {
	  esp.print_string(get_Rom_Data(esp_commands,e_mode));esp.write(char(a));esp.print_string("\r\n");
  }
  void set_esp_as_wifi(char* ssid, char* password, uint8_t security_level)
  {
	  if (security_level == OPEN)
	  {
		  esp.print_string(get_Rom_Data(esp_commands,e_config));esp.write('=');esp.write('"');esp.write(ssid);
		  esp.write('"');esp.write(',');esp.write('"');esp.write(password);esp.write('"');esp.write(',');
		  esp.print_string("5,0,4\r\n");
	  }
	  else if (security_level == CLOSED)
	  {
		  esp.print_string(get_Rom_Data(esp_commands,e_config));esp.write('=');esp.write('"');esp.write(ssid);
		  esp.write('"');esp.write(',');esp.write('"');esp.write(password);esp.write('"');esp.write(',');
		  esp.print_string("5,3,4\r\n");
	  }
  }

  void set_esp_local_ipaddress(char* ipaddress)
  {
	  esp.print_string(get_Rom_Data(esp_commands,e_ip));esp.write('=');esp.write('"');esp.write(ipaddress);esp.write('"');
	  esp.print_string("\r\n");
  }
  
  void set_connectionMode(int a)
  {
	  esp.print_string(get_Rom_Data(esp_commands,e_type));  esp.print_string("\r\n");
  }

  void setup_esp_as_server()
  {
	  esp.print_string(get_Rom_Data(esp_commands,e_server));  esp.print_string("\r\n");	  
  }

  void chheck_esp_connection_status()
  {
	  
	  esp.print_string(get_Rom_Data(esp_commands,e_status));  esp.print_string("\r\n");
  }
  
  //-----------------------------------------------------------------------------------------
  void esp_start_tcp_connection(char* server_url,char* port_no)
  {
	  esp.print_string(get_Rom_Data(esp_commands,e_start)); esp.write('"');esp.print_string(server_url);
	  esp.write('"');esp.write(','); esp.print_string(port_no);  esp.print_string("\r\n");
  }
  //------------------------------------------------------------------------------------------
  void esp_join_hotspot(char* hotspot_name,char* hotspot_password)
  {
	  esp.print_string(get_Rom_Data(esp_commands,e_join));esp.write('=');esp.write('"');esp.write(hotspot_name); esp.write('"');esp.write(',');
	  esp.write('"');esp.write(hotspot_password); esp.write('"');  esp.print_string("\r\n");
  }
  //------------------------------------------------------------------------------------------
  //------------------------------------------------------------------------------------------
  void connect_esp_to_MQTT_server(char*  username,char* password,char* serial_no)
  {
	  const char protocol[10]="MQIsdp", protocol_lvl=0X03, flag=0xC2; int data_len;
	  unsigned int keep_alive=60;
	  unsigned char  encodedByte;
	  unsigned short protocol_len, clid_len,username_len, pw_len;

	  protocol_len=strlen(protocol);
	  clid_len= strlen(serial_no);
	  username_len=strlen(username);
	  pw_len=strlen(password);
	  data_len = protocol_len+clid_len+username_len+pw_len+12;
	  unsigned long x=data_len;
	  short len=data_len+2;

	  esp.print_string(get_Rom_Data(esp_commands,e_send));esp.write('=');
	  esp.print_string(len);esp.print_string("\r\n");
	  esp.write(0x10);     //control byte for MQTT connection
	  do
	  {
		  encodedByte = x%128;
		  x = x/128;
		  if ( x > 0 ){encodedByte |= 128;}
		  esp.write(encodedByte);
	  }while ( x > 0 );

	  esp.write(protocol_len>>8);
	  esp.write(protocol_len & 0xFF);
	  esp.print_string(protocol);

	  esp.write(protocol_lvl);
	  esp.write(flag);
	  esp.write(keep_alive>>8);
	  esp.write(keep_alive & 0xFF);

	  esp.write(clid_len>>8);
	  esp.write(clid_len & 0xFF);
	  esp.print_string(serial_no);

	  esp.write(username_len>>8);
	  esp.write(username_len & 0xFF);
	  esp.print_string(username);

	  esp.write(pw_len>>8);
	  esp.write(pw_len & 0xFF);
	  esp.print_string(password);
  }
  //------------------------------------------------------------------------------------------
  //------------------------------------------------------------------------------------------
  void esp_publish_data(char* topic,char* message)
  {
	  int data_len;
	  unsigned short topic_len,message_len;
	  unsigned char  encodedByte;
	  
	  topic_len=strlen(topic);
	  message_len=strlen(message);
	  data_len = message_len+topic_len+2;
	  short len =data_len+2;
	  esp.print_string(get_Rom_Data(esp_commands,e_send));esp.write('=');esp.print_string(len);  esp.print_string("\r\n");
	  esp.write(0x30);     //control byte for MQTT connection
	  unsigned long x=data_len;
	  do
	  {
		  encodedByte = x%128;
		  x = x/128;
		  if ( x > 0 ){encodedByte |= 128;}
		  esp.write(encodedByte);
		  
	  }while ( x > 0 );

	  esp.write(topic_len>>8);
	  esp.write(topic_len & 0xFF);
	  esp.print_string(topic);
	  esp.print_string(message);
  }
  //------------------------------------------------------------------------------------------
  
  void esp_mqtt_ping()
  {
	  char ping=0x00;
	  esp.listen();
	  _delay_ms(10);
	  esp.print_string(get_Rom_Data(esp_commands,e_send));esp.write('=');esp.print_string('2');esp.print_string("\r\n");
	  
	  esp.write(0xC0);

	  esp.write(ping);
	  
  }
  
  void esp_mqtt_subscribe(char* topic)
  {
	  int data_len;
	  uint8_t topic_len,message_len, QoS=0x00,packet_id=0x01;
	  unsigned char  encodedByte;

	  topic_len=strlen(topic);
	  short len=data_len+2;
	  esp.print_string(get_Rom_Data(esp_commands,e_send));esp.write('=');esp.print_string(len);esp.write(0x0D);esp.write(0x0A);
	  _delay_ms(100);
	  esp.write(0x82);
	  unsigned long x=data_len;
	  do 
	  {
		  encodedByte = x%128;
		  x = x/128;
		  // if there are more data to encode, set the top bit of this byte
		  if ( x > 0 ){encodedByte |= 128;}
		  esp.write(encodedByte);
		  
	  }while ( x > 0 );

	  esp.write(packet_id>>8);
	  packet_id=0x01;
	  esp.write(packet_id&0xFF);
	  
	  esp.write(topic_len>>8);
	  esp.write(topic_len & 0xFF);
	  esp.print_string(topic);
	  
	  esp.write(QoS);
	  esp.write(0x1A);
  }