/*
 * io.c
 *
 * Created: 27/03/2017 12:39:20
 *  Author: Denis
 */ 

#include "digital.h"
#include "system.h"

#include <avr/interrupt.h>

/*
 * PD0 - pull up button (interrupt)
 * PD1 - led red = x(t)
 * PD2 - led blue = v(t) 
 * PD3 - led yellow = key
 */
void digital_init(void)
{
	/* Set PB0 as output (used for showing when the system is running). */
	DDRB |= (1 << DDB0);
	
	/* Set PD2 as input */
	/* DDRD &= ~(1 << DDD2); */
	
	/* Turn on the pull up for pin PD2. */
	PORTD |= (1 << PORTD2);
	
	/* Configure external interrupts on pin PD2 (INT0) to detect falling edge. */
	EICRA |= (1 << ISC01);
	
	/* Enable external interrupts on pin PD2 (INT0). */
	EIMSK |= (1 << INT0);
}

ISR(INT0_vect)
{
	if(!system_running)
	{
		system_start();
	}
	else
	{
		system_stop();
	}
}