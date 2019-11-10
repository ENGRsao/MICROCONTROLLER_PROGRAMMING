/*
 * ZigbeeETRX357.c
 *
 * Created: 7/31/2018 2:40:08 PM
 * Author : ENGR SAO
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define BAUD_rate 19200
#define BAUD_prescaler (F_CPU/16/BAUD_rate - 1)

int physical_pin [25] = {2,3,4,5,6,8,9,10,11,12,11,12,14,15,16,17,18,22,23,24,26,27,28,29,30};
char* pin_bit [25] = {"15","16","17","7","B","C","0","1","2","3","4","5","6","9","A","12","13","14","11","10","F","E","D"};
enum {A0,A1,A2,A3};
char* ADC_bit[4] = {"1F","20","21","22"};
char* set_ADC [4] = {"D","E","F","11"};

void USART_init(unsigned int UBRR)
{
	UBRR0H = (unsigned char)(UBRR >> 8);
	UBRR0L = (unsigned char)UBRR;	
	UCSR0B = (1<<TXEN0) | (1<< RXEN0) ;
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
}

unsigned char Usart_receive_byte(void)
{
	while (!(UCSR0A & (1<<RXC0)))
	;
	return UDR0;
}

void Usart_transmit_byte(unsigned char u_data)
{
	while (!(UCSR0A & (1<<UDRE0)) )
	;
	UDR0 = u_data;
}

void send_data(const char* word)
{
	while (*word != '\0')
	{
		Usart_transmit_byte(*word++);
	}

}

void send_char(const char val)
{
			Usart_transmit_byte(val);
}

char* get_pin_bit(int pin)
{
	int k = 0;
	while(!(physical_pin[k] == pin)){k++;}
		return pin_bit[k];	
}

void establishNetwork()
{	
	send_data("AT+EN\r");
}
void scanNetwork()
{
	send_data("AT+SN\r");
}
void scan_for_available_Network()
{
	send_data("AT+PANSCAN\r");
}
void restore_Factory_Default()
{		
		send_data("AT&F\r");
}
void bootloader_Mode()
{
		send_data("AT+BLOAD\r");		
} 
	
void allowConnection(char* password)
{
	send_data("ATS0A5=0:"); send_data(password);send_data("\r");	
}

void disallowJoining(char* password)
{
	send_data("ATS0A5=1:"); send_data(password);send_data("\r");	
}
void check_device()
{
	send_data("AT\r");
}
void device_firmware()
{
	send_data("ATI\r");
}
void check_device_connection()
{
	send_data("AT+N\r");
}
void join_network(char* channel, char* network_ID)
{
	send_data("AT+JPAN:");send_data(channel);send_data(network_ID);send_data("\r");
}

void announcePresence()
{
	send_data("AT+ANNCE\r");
}
void activate_security()
{
	send_data("ATS0A8=1\r");
	_delay_ms(100);
	send_data("ATS0A4=1\r");
	_delay_ms(100);
	send_data("ATS0A2=1\r");
	_delay_ms(100);
}
void request_Parent()
{
	send_data("ATS07?\r");	
}
void remote_set_device_function(char* address,int a)
{
	if (a == 1) //function as a router
	{
		send_data("ATREMS:");send_data(address);send_data(":0E=0\r");
		_delay_ms(10);
		send_data("ATREMS:");send_data(address);send_data(":0F=0\r");
	}
	else if (a == 2) //function as end device
	{
		
		send_data("ATREMS:");send_data(address);send_data(":0E=0\r");
		_delay_ms(10);
		send_data("ATREMS:");send_data(address);send_data(":0F=1\r");
	}
	
	else if (a == 3) //function as sleepy end device
	{		
		send_data("ATREMS:");send_data(address);send_data(":0E=1\r");
		_delay_ms(10);
		send_data("ATREMS:");send_data(address);send_data(":0F=0\r");
	}
	
	else if (a == 4) //function as mobile end device
	{		
		send_data("ATREMS:");send_data(address);send_data(":0E=1\r");
		_delay_ms(10);
		send_data("ATREMS:");send_data(address);send_data(":0F=1\r");
	}
}
void set_device_function(int a)
{
	if (a == 1) //function as a router
	{
		send_data("ATS0AE = 0\r");
		_delay_ms(10);
		send_data("ATS0AF = 0\r");
	}
	else if (a == 2) //function as end device
	{
		send_data("ATS0AF = 1\r");
		send_data("ATS0AE = 0\r");
	}
	
	else if (a == 3) //function as sleepy end device
	{
		send_data("ATS0AE = 1\r");
		send_data("ATS0AF = 0\r");
	}
	
	else if (a == 4) //function as mobile end device
	{
		send_data("ATS0AE = 1\r");
		send_data("ATS0AF = 1\r");
	}
}
void remote_set_device_NAME(char* address,char* device_name,char* password)//name up to 16 character
{
	send_data("ATREMS:");send_data(address);send_data(",0B=");send_data(device_name);send_data(":");send_data(password);send_data("\r");
}
void set_device_NAME(char* device_name,char* password)//name up to 16 character
{
	send_data("ATS0B=");send_data(device_name);send_data(":");send_data(password);send_data("\r");
}
void remote_set_password(char* address,char* new_password,char* old_password)
{
	send_data("ATREMS:");send_data(address);send_data(",0C=");send_data(new_password);send_data(":");send_data(old_password);send_data("\r");
}
void set_password(char* new_password,char* old_password)
{
	send_data("ATS0C=");send_data(new_password);send_data(":");send_data(old_password);send_data("\r");
}
void disable_Prompt()
{
	send_data("ATS0EC=1\r"); //disable SEQ prompt
	send_data("ATS0EA=1\r"); //disable SR prompt
	send_data("ATS0E7=1\r"); //disable NACK prompt
	send_data("ATS0E6=1\r"); //disable ACK prompt
	send_data("ATS0E3=1\r"); //disable JPAN prompt
	send_data("ATSOE1=1\r"); //disable OK prompt
	send_data("ATS0E0=1\r"); //disable ERROR prompt
	send_data("ATS0F8=1\r"); //disable SWRITE prompt
	send_data("ATS0E5=1\r"); //disable all multiCAST and UNICAST prompt	
}

void notify_an_exition()
{
	send_data("ATS0F6=1\r"); //EnabLe LEFTNODE prompt
}
void remote_disconnect_device(char* network_ID)
{
	send_data("AT+DASSR:");send_data(network_ID);send_data("\r");//disconnect device
}
void disconnect_device()
{
	send_data("AT+DASSL:");send_data("\r");//disconnect device
}

int set_baudRate(unsigned long value)
{
	int i = 0;
	switch (value)
	{
		case 1200: send_data("ATS12=00\r");i = 1;break;
		case 2400: send_data("ATS12=01\r");i = 1;break;
		case 4800: send_data("ATS12=02\r");i = 1;break;
		case 9600: send_data("ATS12=03\r");i = 1;break;
		case 14400: send_data("ATS12=04\r");i = 1;break;
		case 19200: send_data("ATS12=05\r");i = 1;break;
		case 28800: send_data("ATS12=06\r");i = 1;break;
		case 38400: send_data("ATS12=07\r");i = 1;break;
		case 50000: send_data("ATS12=08\r");i = 1;break;
		case 57600: send_data("ATS12=09\r");i = 1;break;
		case 76800: send_data("ATS12=0A\r");i = 1;break;
		case 100000: send_data("ATS12=0B\r");i = 1;break;
		case 115200: send_data("ATS12=0C\r");i = 1;break;
		default: i = 0;break;
	}
	return i;
}
void sendUnicast_message(char* address,char* message)
{
	send_data("AT+UCAST:");send_data(address);send_char('=');send_data(message);send_data("\r");	
}
void sendBroadcast_message(char* hop,char* message)
{
	send_data("AT+BCAST:");send_data(hop);send_char(',');send_data(message);send_data("\r");
}

void remote_setpin_mode(char* address,char* password,int pin, int mode)
{
	char* i = get_pin_bit(pin);
	if (mode == 0) //set as output
	{
		send_data("ATREMS:");send_data(address);send_data("16");send_data(i);send_data("= 0,");send_data(password);
	}
	
	else if (mode == 1)//set as input
	{
		send_data("ATREMS:");send_data(address);send_data("16");send_data(i);send_data("= 1,");send_data(password);
	}
}
void setpin_mode(int pin, int mode)
{
	char* i = get_pin_bit(pin);
	if (mode == 0)
	{
		send_data("ATS16");send_data(i);send_data("= 0\r");
	}
	
	else if (mode == 1)
	{
		send_data("ATS16");send_data(i);send_data("= 1\r");
	}
}
void remote_digital_Read(char* address,int pin)
{
	char* i = get_pin_bit(pin);
	send_data("ATREMS:");send_data(address);send_data(",");send_data(i);send_data("?\r");
}

void digital_Read(int pin)
{
	char* i = get_pin_bit(pin);
	send_data("ATS1A");send_data(i);send_data("?\r");
}

void remote_digital_Write(char* address,int pin,int val)
{
	char* i =  get_pin_bit(pin);
	if (val == 0){
		send_data("ATREMS:");send_data(address);send_data(",");send_data("18");send_data(i);send_data("=0\r");}
	else if (val == 0){
		send_data("ATREMS:");send_data(address);send_data(",");send_data("18");send_data(i);send_data("=1\r");
	}
}

void digital_Write(int pin,int val)
{
	char* i =  get_pin_bit(pin);
	if (val == 0)
		{
			send_data("ATS18");send_data(i);send_data("=0\r");
			}
	else if (val == 0)		
		{
			send_data("ATS18");send_data(i);send_data("=1\r");
			}
}
void remote_set_ADC_reference(char* address) //set ADC reference of module to be 1.2V
{
	send_data("ATREM:");send_data(address);send_data(",158=1\r");
}
void set_ADC_refeence()//set ADC reference of module to be 1.2V
{
	send_data("ATS158=1\r");
}
void remote_enableADC(char* address,int pin)
{
	char* val = set_ADC[pin];	
	send_data("ATREMS:");send_data(address);send_data(",");send_data("15");send_data(val);send_data("= 1");send_data("\r");	
}

void enableADC(int pin)
{
	char* val = set_ADC[pin];
	
	send_data("ATS15");send_data(val);send_data("= 1");send_data("\r");
}

void remote_Read_ADC_value(char* address, int pin)
{
	char* val = ADC_bit[pin];
	send_data("ATREMS:");send_data(address);send_data(",");send_data(val);send_data("?");
		
}

void Read_ADC_value(int pin)
{	
	char* val = ADC_bit[pin];
	send_data("ATS");send_data(val);send_data("?");
	
	}

long get_Decimal_Value(char* a)
{
	long result = 0; int t = strlen(a);int index = 0;
	for (int i = t; i >= 0;i--)
	{
		char b = a[i];
		if ( (b == '0')||(b == '1')||(b == '2')||(b == '3')||(b == '4')||(b == '5')||(b == '6')||(b == '7')||(b == '8')||(b == '9'))
		{
			int k = b - '0';
			result += (k * pow(16,index))+ 1;
		}
		else if (b == 'A')
		{
			result += (10 * pow(16,index));
		}
		else if (b == 'B')
		{
			result += (11 * pow(16,index));
		}
		else if (b == 'C')
		{
			result += (12 * pow(16,index));
		}
		else if (b == 'D')
		{
			result += (13 * pow(16,index));
		}
		else if (b == 'E')
		{
			result += (14 * pow(16,index));
		}
		else if (b == 'F')
		{
			result += (15 * pow(16,index));
		}
		else
		{
			result += (0 * pow(16,result));
		}
		t++;
	}
	return result;
}

