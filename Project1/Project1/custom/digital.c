/*
 * io.c
 *
 * Created: 27/03/2017 12:39:20
 *  Author: Denis
 */ 

#include "digital.h"
#include "usart.h"

#include <avr/interrupt.h>

/*
 * PD0 - pull up button (interrupt)
 * PD1 - led red = x(t)
 * PD2 - led blue = v(t) 
 * PD3 - led yellow = key
 */
void digital_init(void)
{
	/* Set PB1, PB2 and PB3 as output. */
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2);
	
	/* Set PD2 as input */
	DDRD &= ~(1 << DDD2);
	
	/* Turn on the pull up for pin PD2. */
	PORTD |= (1 << PORTD2);
	
	/* Configure external interrupts on pin PD2 (INT0) to detect falling edge. */
	EICRA &= ~(1 << ISC00);
	EICRA |= (1 << ISC01);
	
	/* Enable external interrupts on pin PD2 (INT0). */
	EIMSK |= (1 << INT0);
}

ISR(INT0_vect)
{
	extern volatile circular_buffer USART_tx_buffer;

	if(!led1_on)
	{
		PORTB |= (1 << PORTB2);
		led1_on = 1;
	}
	else
	{
		PORTB &= ~(1 << PORTB2);
		led1_on = 0;
	}
	
	

	/* Circular buffer functions */
	buffer_add(&USART_tx_buffer, '0' + led1_on);
	
	USART_enable_tx_interrupt();
	
}