/*
 * sim_mqtt_Library.c
 *
 * Created: 7/30/2018 6:49:58 PM
 * Author : ENGR SAO
 */ 

#include <avr/io.h>
#include <SoftwareSerial.h>
#include <util/delay.h>
#define F_CPU 16000000UL

SoftwareSerial sim(2,3);

char buffer[15];

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
  
  char* get_Rom_Data(const char* Table_Details[],int i)
  {
	  strcpy_P(buffer,(char*) pgm_read_word(&Table_Details[i]));
	  return buffer;
  }

  short sim_check()
  {
	  sim.write(get_Rom_Data(sim_commands,s_check));sim.write(0x0D);sim.print_string("\r\n");
  }

  void sim_status()
  {
	  sim.write(get_Rom_Data(sim_commands,s_status));sim.print_string("\r\n");	  
  }
  //------------------------------------------------------------------------------------------
  void shut_sim_TCP_connection()
  {
	  sim.write(get_Rom_Data(sim_commands,s_shut));sim.print_string("\r\n");	  
  }
  
  void sim_set_apn(char* sim_apn)
  {
	  sim.write(get_Rom_Data(sim_commands,s_apn));sim.write('=');sim.write('"');sim.print_string(sim_apn);sim.write('"');
	  sim.write(',');sim.write('"');sim.write('"');sim.write(',');sim.write('"');
	  sim.write('"');sim.print_string("\r\n");
	  
  }
  //------------------------------------------------------------------------------------------
  void activate_sim_gprs()
  {
	  sim.write(get_Rom_Data(sim_commands,s_gprs));sim.print_string("\r\n");
	_delay_ms(50);
  }
  //---------------------------------------------------------------------------------------
  void set_sim_ipaddress()
  {
	  sim.write(get_Rom_Data(sim_commands,s_config));sim.print_string("\r\n");
	  _delay_ms(50);
  }
  //---------------------------------------------------------------------------------------
  void set_sim_ssl(unsigned int val)
  {
	  sim.write(get_Rom_Data(sim_commands,s_ssl));sim.write('=');sim.write(val);sim.print_string("\r\n");
	  _delay_ms(50);
  }
  //----------------------------------------------------------------------------------------
  //------------------------------------------------------------------------------------------
  void sim_start_TCP_connection(char* server_url,char* port_no )
  {  sim.write(get_Rom_Data(sim_commands,s_start)); sim.write('"');sim.print_string(server_url);
	  sim.write('"');sim.write(','); sim.write('"');sim.print_string(port_no);
	  sim.write('"');sim.print_string("\r\n");
	  _delay_ms(50);
  }

  void connect_sim_to_mqtt(char* username,char* password, char* serial_no)
  {
	  const char protocol[10]="MQIsdp", protocol_lvl=0X03, flag=0xC2;int data_len;
	  unsigned int keep_alive=60;
	  unsigned char  encodedByte;
	  unsigned short protocol_len, clid_len,username_len, pw_len;
	  char* client_id="sim"+serial_no;

	  sim.write(get_Rom_Data(sim_commands,s_send));sim.print_string("\r\n");
	  _delay_ms(50);
	  sim.write(0x10);     //control byte for MQTT connection
	  protocol_len=strlen(protocol);
	  clid_len=strlen(client_id);
	  username_len=strlen(username);
	  pw_len=strlen(password);
	  data_len=protocol_len+clid_len+username_len+pw_len+12;
	  unsigned long x=data_len;
	  do
	  {
		  encodedByte = x%128;
		  x = x/128;
		  if ( x > 0 ){encodedByte |= 128;}
		  sim.write(encodedByte);
		  
	  }while ( x > 0 );

	  sim.write(protocol_len>>8);
	  sim.write(protocol_len & 0xFF);
	  sim.print_string(protocol);

	  sim.write(protocol_lvl);
	  sim.write(flag);
	  sim.write(keep_alive>>8);
	  sim.write(keep_alive & 0xFF);

	  sim.write(clid_len>>8);
	  sim.write(clid_len & 0xFF);
	  sim.print_string(client_id);

	  sim.write(username_len>>8);
	  sim.write(username_len & 0xFF);
	  sim.print_string(username);

	  sim.write(pw_len>>8);
	  sim.write(pw_len & 0xFF);
	  sim.print_string(password);
	  sim.write(0x1A);
	  _delay_ms(1000);

  }
  //---------------------------------------------------------------------------------
  short publish_to_mqtt(char* topic, char* msg), char* topic)
  {
	  int data_len;
	  unsigned short topic_len,message_len;
	  unsigned char  encodedByte;
	  sim.listen();
	  _delay_ms(10);
	  sim.write(get_Rom_Data(sim_commands,s_send));sim.write(0x0D);sim.write(0x0A);
	  _delay_ms(50);
	  
	  sim.write(0x30);     //control byte for MQTT connection
	  topic_len=strlen(topic);//strlen(topic);
	  message_len=strlen(msg);
	  data_len=message_len+topic_len+2;

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
	  sim.print_string(topic);
	  
	  sim.print_string(msg);

	  sim.write(0x1A);
	  _delay_ms(10);
	  
  }
  //---------------------------------------------------------------------------------
  void ping_mqtt_server()
  {
	  char ping=0x00;
	sim.write(get_Rom_Data(sim_commands,s_send));sim.write(0x0D);sim.write(0x0A);
	  _delay_ms(50);
	  
	  sim.write(0xC0);

	  sim.write(ping);
	  
	  sim.write(0x1A);	  
	    }

  //-----------------------------------------------------------------------------------------
  void subscribe_to_mqtt(char* topic)
  {
	  int data_len;
	  unsigned int topic_len,message_len, QoS =0x00,packet_id=0x01;
	  unsigned char  encodedByte;

	  topic_len=strlen(topic);//strlen(topic);
	  data_len=topic_len+5;
	  short len = data_len+2;
	  
	  _delay_ms(20);
	  
	  sim.write(get_Rom_Data(sim_commands,s_send));sim.write(0x0D);sim.write(0x0A);
	  _delay_ms(50);
	  sim.write(0x82);
	  unsigned long x=data_len;
	  do
	  {
		  encodedByte = x%128;
		  x = x/128;
		  // if there are more data to encode, set the top bit of this byte
		  if ( x > 0 ){encodedByte |= 128;}
		  sim.write(encodedByte);
		  
	  }while ( x > 0 );

	  sim.write(packet_id >> 8);
	  sim.write(packet_id & 0xFF);
	  
	  sim.write(topic_len >> 8);
	  sim.write(topic_len & 0xFF);
	  sim.print_string(topic);
		    
	  sim.write(QoS);  //QoS
	  sim.write(0x1A);
	 }



