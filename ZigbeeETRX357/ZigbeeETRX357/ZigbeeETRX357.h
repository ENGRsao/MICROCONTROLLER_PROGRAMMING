/*
 * ZigbeeETRX357.h
 *
 * Created: 8/1/2018 11:12:50 PM
 *  Author: ENGR SAO
 */ 


#ifndef ZIGBEEETRX357_H_
#define ZIGBEEETRX357_H_

void USART_init(unsigned int UBRR); //initialize Serial communication by starting BAUD rate

unsigned char Usart_receive_byte(void); //receive data via Serial/UART communication

void Usart_transmit_byte(unsigned char u_data); //transmit a byte via Serial communication

void send_data(const char* word); //send String data via Serial/UART communication

void send_char(const char val); // send a constant character via USART

char* get_pin_bit(int pin); //return corresponding bit of a given physical pin of the module

void establishNetwork(); //Establish a Network with a coordinator

void scanNetwork(); //scan for devices connected in the same network

void scan_for_available_Network(); // scan through the network channel for available network to connect to

void restore_Factory_Default(); //restore device to factory settings 

void bootloader_Mode(); // make device ready for Boot-loading 

void allowConnection(char* password); //set Coordinator to open room for new device connection to its network

void disallowJoining(char* password); //set Coordinator not to allow any new device to join

void check_device(); //check if device is still available 

void device_firmware(); // check Telegesis firmware type available on the device

void check_device_connection(); // check if network is still connected and to which network

void join_network(char* channel, char* network_ID); // set device to join a specific network

void announcePresence(); // set a device to announce its presence to all other devices present in the network

void request_Parent(); //request a device for the network ID to which it is connected

void remote_set_device_function(char* address,int a); //remotely set the function of a device in a network; 1- router, 2- end devices 3- sleepy device 4- mobile end device

void set_device_function(int a); //set a device function in a network;1- router, 2- end devices 3- sleepy device 4- mobile end device

void remote_set_device_NAME(char* address,char* device_name,char* password); //remotely set name of a device; NOTE: Name can up to 16 character

void set_device_NAME(char* device_name,char* password); //set the name of a device; Name can be up to 16 character

void remote_set_password(char* address,char* new_password,char* old_password); //remotely set a device password

void set_password(char* new_password,char* old_password); //set device password

void disable_Prompt(); //disable some prompt on the coordinator

void notify_an_exition(); //notify the coordinator when a device leaves a node

void remote_disconnect_device(char* network_ID); //remotely disconnect a device

void disconnect_device(); // disconnect a device from a network

int set_baudRate(unsigned long value); // set_device BaudRate

void sendUnicast_message(char* address,char* message); //send a UNICAST message to a device

void sendBroadcast_message(char* hop,char* message); //send a broadcast message to the network

void remote_setpin_mode(char* address,char* password,int pin, int mode); // remotely format pin as digital pin

void setpin_mode(int pin, int mode); //format device pin as input or output

void remote_digital_Read(char* address,int pin); // remotely read a digital pin

void digital_Read(int pin); //read a device digital pin


void remote_digital_Write(char* address,int pin,int val); //remotely write to a digital pin

void digital_Write(int pin,int val); //write to a device digital pin to give its state


void remote_set_ADC_reference(char* address); //remotely set internal ADC reference voltage for a device

void set_ADC_refeence(); //set internal voltage as ADC reference

void remote_enableADC(char* address,int pin);// remotely enable ADC for measurement

 void enableADC(int pin); //enable device ADC for measurement
 
 void remote_Read_ADC_value(char* address, int pin); //remotely read a specific ADC pin of a device
 
 void Read_ADC_value(int pin); //measure device ADC pin

 long get_Decimal_Value(char* a); //convert ADC value Decimal value
 
 void activate_security();
 
#endif /* ZIGBEEETRX357_H_ */