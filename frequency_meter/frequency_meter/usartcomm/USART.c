/*
 * USART.c
 *
 * Created: 8/18/2018 1:02:33 AM
 *  Author: ENGR SAO
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define BAUD_rate 9600
#define BAUD_prescaler (F_CPU/16/BAUD_rate - 1)

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