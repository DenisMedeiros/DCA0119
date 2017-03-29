/*
 * CFile1.c
 *
 * Created: 22/03/2017 22:53:02
 *  Author: Denis
 */ 

#include "counters.h"
#include "usart.h"
#include "adc.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <math.h>

#define TIMER_1_SEC_TICKS 125

void counters_init(void)
{	
	
	/** Counter0 - 1 seg counter (USART) */
	
	/* Set Counter0 to CTC mode. */
	TCCR0A |= (1 << WGM01);

	/* Set prescale to 1024 and start the timer. */
	TCCR0B |= (1 << CS00) | (1 << CS02);
	
	/* Enable overflow interrupt for counter 0. */
	TIMSK0 |= (1 << OCIE0A);
	
	/* Set the CTC value to compare. */
	/* 125 is useful to get exactly 1 second using interrupts. */
	OCR0A = 125;
	
	/** Counter1 - PWM generator */
	
	/* Set PD3 (driver) and PB0 and PB1 (LEDS) as output (PWM). */
	//DDRD |= (1 << DDD3);
	DDRB |= (1 << DDB1) | (1 << DDB2);
	
	/* Configure counter 1 as fast PWM generator (8-bit resolution). */
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
	
	/* Configure prescaller to 1024 and start the timer. */
	TCCR1B |= (1 << WGM12) | (1 << CS10) | (1 << CS12);
	
	/* Set the initial PWM duty to 0 (system stopped).  */
	OCR1A = 0;
	OCR1B = 0;
	
}

/* Start the Counter 1 (PWM generator). */
void counters_start(void)
{
	/* Configure counter 1 as fast PWM generator (8-bit resolution). */
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
	
	/* Configure prescaller to 1024 and start the timer. */
	TCCR1B |= (1 << WGM12) | (1 << CS10) | (1 << CS12);
}

/* Stop the Counter 1 (PWM generator). */
void counters_stop(void)
{
	/* Set the PWM duty */
	OCR1A = 0;
	OCR1B = 0;
	
	/* Configure counter 1 as fast PWM generator (8-bit resolution). */
	TCCR1A &= ~(1 << COM1A1) & ~(1 << COM1B1) & ~(1 << WGM10);
	
	/* Configure prescaller to 1024 and start the timer. */
	TCCR1B &= ~(1 << WGM12) & ~(1 << CS10) & ~(1 << CS12);
	
}


/* Interrupt for counter 0. */
ISR(TIMER0_COMPA_vect)
{
	float percentV, percentX;
	uint8_t v, x;
	
	if(ticks > TIMER_1_SEC_TICKS)
	{
		ticks = 0;
		if(system_running)
		{
			
			total_time_running++;
			
			if(total_time_running > 180)
			{
				system_stop();
				return;
			}
			
			/* Do a weighting depending on the sensor value. */
			percentX = (sensor_value/255.0)*100.0;
			percentV = ((100.0 - percentX)*dryer_mode1(total_time_running)/100.0);
			
			x = roundf(percentX);
			v = roundf(percentV);
			
			/* Set color of LED 2 and speed of the motor */
			change_duty_led_v(roundf(255.0*percentV/100.0));
			
			/* Concert values to char to be sent by USART. */
			itoa(total_time_running, (char*) t_s, 10);
			itoa(x, (char*) x_s, 10);
			itoa(v, (char*) v_s, 10);

			buffer_put_string(&USART_tx_buffer, (char*) t_s);
			buffer_add(&USART_tx_buffer, ';');
			buffer_put_string(&USART_tx_buffer, (char*) x_s);
			buffer_add(&USART_tx_buffer, ';');
			buffer_put_string(&USART_tx_buffer, (char*) v_s);
			buffer_add(&USART_tx_buffer, '+');
			
			USART_enable_tx_interrupt();
		}
		else
		{
			buffer_add(&USART_tx_buffer, '-');
			USART_enable_tx_interrupt();
		}
	}
	
	ticks++;
}

/* Change duty for LED 1 according to sensor value. */
void change_duty_led_x(uint8_t duty)
{	
	TCCR1B &= ~(1 << CS10) & ~(1 << CS12);
	OCR1B = duty;
	TCCR1B|= (1 << CS10) | (1 << CS12);
}

/* Change duty cycle of LED 2. This PWM signal is also used as
 *  input for the optocoupler.
 */
void change_duty_led_v(uint8_t duty)
{
	TCCR1B &= ~(1 << CS10) & ~(1 << CS12);
	OCR1A = duty;
	TCCR1B|= (1 << CS10) | (1 << CS12);
}

