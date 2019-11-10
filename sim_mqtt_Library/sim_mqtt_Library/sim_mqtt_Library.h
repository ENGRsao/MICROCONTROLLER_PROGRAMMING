/*
 * sim_mqtt_Library.h
 *
 * Created: 7/30/2018 9:24:22 PM
 *  Author: ENGR SAO
 */ 


#ifndef SIM_MQTT_LIBRARY_H_
#define SIM_MQTT_LIBRARY_H_

 short sim_check();// check if SIM is ready to function
	 
  void sim_status(); // check for SIM connection status
	  
  void shut_sim_TCP_connection(); // stop TCP connection for SIM module

	void sim_set_apn(char* sim_apn) ;// specify SIM APN
  
	void activate_sim_gprs() ; //Activate GPRS connection for SIM module

	void set_sim_ipaddress(); //set SIM IP address

	void set_sim_ssl(unsigned int val); //Activate or Deactivate SSL connection for SIM module

	void sim_start_TCP_connection(char* server_url,char* port_no ); // begin TCP connection with SIM module stating the server URL and port number 

	void connect_sim_to_mqtt(char* username,char* password, char* serial_no); //connect SIM module to MQTT server by passing  in the username and password and serial number

	short publish_to_mqtt(char* topic, char* msg), char* topic) ; //publish message to a particular MQTT broker by passing the topic and message to be sent
 
	void ping_mqtt_server() ;//send a ping message to the MQTT server to avoid close connection 

	void subscribe_to_mqtt(char* topic); //subscribe to MQTT server after connection is established by passing in the topic



#endif /* SIM_MQTT_LIBRARY_H_ */