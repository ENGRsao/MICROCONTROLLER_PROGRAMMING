/*
 * frequency_meter.c
 *
 * Created: 8/17/2018 8:22:31 PM
 * Author : ENGR SAO
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "USART.h"

ISR (TIMER1_CAPT_vect)
{
	unsigned int i = ICR1L;
	unsigned int k = ICR1H;
	Usart_transmit_byte(i);
	Usart_transmit_byte(k);
	
}

int main(void)
{
    /* Replace with your application code */
	USART_init(103);
	send_data("FREQUENCY METER");
	TCCR1B |= (1<<ICES1)| (1<<ICNC1);	
	TIMSK1 |= (1<<TOIE1);
	
	/*TCCR0B |= (1<<CS01)|(0<<CS00);*/
	
    while (1) 
    {
		sei();
    }
}

