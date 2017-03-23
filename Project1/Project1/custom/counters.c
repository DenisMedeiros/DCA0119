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
	
	char t_s[8], x_s[8], v_s[8];
	uint8_t t = 0, v = 0, x = 0;
	
	if(ticks > TIMER_1_SEC)
	{
		t++;
		v = t+2;
		x = t/2;
		
		itoa(t, t_s, 10);
		itoa(x, x_s, 10);
		itoa(v, v_s, 10);

		buffer_put_string(&USART_tx_buffer, t_s);
		buffer_add(&USART_tx_buffer, ';');
		buffer_put_string(&USART_tx_buffer, x_s);
		buffer_add(&USART_tx_buffer, ';');
		buffer_put_string(&USART_tx_buffer, v_s);
		buffer_add(&USART_tx_buffer, '\n');
		
		USART_enable_tx_interrupt();
		ticks = 0;
	}
	ticks++;
}