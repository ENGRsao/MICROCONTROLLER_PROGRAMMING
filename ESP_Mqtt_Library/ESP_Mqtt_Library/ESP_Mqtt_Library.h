/*
 * ESP_Mqtt_Library.h
 *
 * Created: 7/30/2018 10:28:15 PM
 *  Author: ENGR SAO
 */ 


#ifndef ESP_MQTT_LIBRARY_H_
#define ESP_MQTT_LIBRARY_H_

  void initialize_esp(); //initialize esp to confirm its availability
  void reset_esp(); //reset ESP  
  void disconnect_esp(); //disconnect  ESP from all connected HOTSTOP
   void set_esp_mode(int a); //set mode of the device
     void set_esp_as_wifi(char* ssid, char* password, char* security_level); // set ESP to act as a router with an SSID and password, alo determine if it is open or closed as security level
	   void set_esp_local_ipaddress(char* ipaddress); // set ESP IP ADDRESS when use as a local server
	 void set_connectionMode(int a) ; //set ESP connection mode
	 void setup_esp_as_server(); // set ESP as a local SERVER
	 void esp_status(); // check connection status of the device
	  void esp_start_tcp_connection(char* server_url,char* port_no); //set up TCP connection with ESP stating the server URL and port number
	    void esp_join_hotspot(char* hotspot_name,char* hotspot_password); //make ESP join a Hotspot available with a defined ssid and password
		
		void connect_esp_to_MQTT_server(char*  username,char* password,char* serial_no); //connect ESP to MQTT server of a certain username and password
		 void esp_publish_data(char* topic,char* message); // publish using ESP to a connected server with a given topic and message
		 void esp_mqtt_ping(); // ping the MQtt server to ensure connection
		  void esp_mqtt_subscribe(char* topic); // subscribe to a specific topic on MQTT server


#endif /* ESP_MQTT_LIBRARY_H_ */