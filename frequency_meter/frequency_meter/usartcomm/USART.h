/*
 * USART.h
 *
 * Created: 8/17/2018 11:53:31 PM
 *  Author: ENGR SAO
 */ 


#ifndef USART_H_
#define USART_H_

void USART_init(unsigned int UBRR);

unsigned char Usart_receive_byte(void);

void Usart_transmit_byte(unsigned char u_data);

void send_data(const char* word);

#endif /* USART_H_ */