/*
 * io.c
 *
 * Created: 27/03/2017 12:39:20
 *  Author: Denis
 */ 

#include "digital.h"

#include <avr/interrupt.h>
#include "counters.h"
#include "adc.h"

/*
 * PD0 - pull up button (interrupt)
 * PD1 - led red = x(t)
 * PD2 - led blue = v(t) 
 * PD3 - led yellow = key
 */
void digital_init(void)
{
	/* Set PB0 as output (used for show then the system is running). */
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
		PORTB |= (1 << PORTB0);
		system_running = 1;
		counters_start();
		buffer_add(&USART_tx_buffer, '+');
		adc_start();
	}
	else
	{
		PORTB &= ~(1 << PORTB0);
		system_running = 0;
		sensor_value = 0;
		total_time_running = 0;
		counters_stop();
		buffer_add(&USART_tx_buffer, '-');
		adc_stop();
		change_duty_led_v(0);
		change_duty_led_x(0);
	}
		
	/* Inform o the SCD the status of the system (+ = running, - = stopped). */
	USART_enable_tx_interrupt();
	
}