/*
 * CFile1.c
 *
 * Created: 22/03/2017 22:53:02
 *  Author: Denis
 */ 

#include "counters.h"
#include "usart.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define TIMER_1_SEC 61  

void counters_init(void)
{	
	/* Set counter to normal operation */
	TCCR0A &= ~(1 << WGM00) &  ~(1 << WGM01) &  ~(1 << WGM02);
		
	/* Set prescale to 1024 and start the timer */
	TCCR0B |= (1 << CS00) | (1 << CS02);
	
	/* Enable interrupts */
	TIMSK0 |= (1 << TOIE0);
}

ISR(TIMER0_OVF_vect)
{
	
	if(ticks > TIMER_1_SEC)
	{
		t++;
		v = t+2;
		x = t/2;
		
		itoa(t, (char*) t_s, 10);
		itoa(x, (char*) x_s, 10);
		itoa(v, (char*) v_s, 10);

		buffer_put_string(&USART_tx_buffer, (char*) t_s);
		buffer_add(&USART_tx_buffer, ';');
		buffer_put_string(&USART_tx_buffer, (char*) x_s);
		buffer_add(&USART_tx_buffer, ';');
		buffer_put_string(&USART_tx_buffer, (char*) v_s);
		buffer_add(&USART_tx_buffer, '/');
		
		USART_enable_tx_interrupt();
		ticks = 0;
	}
	ticks++;
}