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
	/* 125 is useful to get exactly 1 sec using interrupts. */
	OCR0A = 125;
	
	/** Counter1 - PWM generator */
	
	/* Set PD3 (driver) and PB0 and PB1 (LEDS) as output (PWM). */
	DDRD |= (1 << DDD3);
	DDRB |= (1 << DDB1) | (1 << DDB2);
	
	/* Configure counter 1 as fast PWM generator (8-bit resolution). */
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
	
	/* Configure prescaller to 1024 and start the timer. */
	TCCR1B |= (1 << WGM12) | (1 << CS10) | (1 << CS12);
	
	/* Set the PWM duty */
	OCR1A = 0;
	OCR1B = 0;
	
}

void counters_stop(void)
{
	/* Stop the timer */
	TCCR0B &= ~(1 << CS00) & ~(1 << CS02);
	
	/* Disable interrupts. */
	TIMSK0 &= ~(1 << TOIE0);
	
	/* Reset the variables */
	t = 0, v = 0, x = 0;
}

void counters_start(void)
{
	/* Set prescale to 1024 and start the timer. */
	TCCR0B |= (1 << CS00) | (1 << CS02);
	
	/* Enable interrupts. */
	TIMSK0 |= (1 << TOIE0);
}

ISR(TIMER0_COMPA_vect)
{
	if(system_running)
	{
		if(ticks > TIMER_1_SEC_TICKS)
		{
			float temp;
			total_time_running++;
			
			/* Do a weighting according to the sensor value */
			temp = (sensor_value/255.0) * dryer_mode1(total_time_running);
			v = roundf(temp);
			
			//t++;
			//v = t+2;
			//x = t/2;
			
			//itoa(t, (char*) t_s, 10);
			//itoa(x, (char*) x_s, 10);
			itoa(v, (char*) v_s, 10);

			buffer_put_string(&USART_tx_buffer, (char*) t_s);
			buffer_add(&USART_tx_buffer, ';');
			buffer_put_string(&USART_tx_buffer, (char*) x_s);
			buffer_add(&USART_tx_buffer, ';');
			buffer_put_string(&USART_tx_buffer, (char*) v_s);
			buffer_add(&USART_tx_buffer, '+');
			
			
			
			
			USART_enable_tx_interrupt();
			ticks = 0;
		}
		ticks++;
	}
}

void change_duty_led1(uint8_t duty)
{
	TCCR1B &= ~(1 << CS10) & ~(1 << CS12);
	OCR1A = duty;
	TCCR1B|= (1 << CS10) | (1 << CS12);
}

void change_duty_led2(uint8_t duty)
{
	TCCR1B &= ~(1 << CS10) & ~(1 << CS12);
	OCR1B = duty;
	TCCR1B|= (1 << CS10) | (1 << CS12);
}


uint8_t dryer_mode1(uint8_t time)
{
	float result_float = 0;
	uint8_t result_int = 0;
	
	if(time <= 30)
	{
		result_int = time;
	}
	else if(time > 30 && time <= 60)
	{
		result_int = 30;
		
	}
	else if(time > 60 && time <= 90)
	{
		result_float = 1.5 * time - 60;
		result_int = roundf(result_float);
	}
	else if(time > 90 && time <= 120)
	{
		result_int = 75;
	}
	else if(time > 120 && time <= 180)
	{
		result_float = -1.25 * time + 225;
		result_int = roundf(result_float);
	}
	else
	{
		result_int = 255;
	}
	
	return result_int;
}