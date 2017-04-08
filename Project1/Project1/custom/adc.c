/*
 * adc.c
 *
 * Created: 27/03/2017 16:18:47
 *  Author: Denis
 */ 

#include "adc.h"
#include "counters.h"

#include <avr/interrupt.h>
#include <stdlib.h>

/* Configures the ADC. */
void adc_init(void)
{
	/* Configure the ADC to use internal Vcc, 8-bit precision and select ADC0 (default). */
	ADMUX |= (1 << REFS0) | (1 << ADLAR);
	
	/* Configure the prescaller to 128 (125 KHz), enable interrupts and enable the ADC. */
	ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);

	/* Configure the ADC to run in free running. */
	//ADCSRB  ~(1 << ADTS0) & ~(1 << ADTS1) & ~(1 << ADTS2);
}

/* Start the ADC. */
void adc_start(void)
{
	ADCSRA |= (1 << ADEN);
	ADCSRA |= (1 << ADSC);
}

/* Stop the ADC. */
void adc_stop(void)
{
	ADCSRA &= ~(1 << ADEN);
	ADCSRA &= ~(1 << ADSC);
}

/* Handle the interrupt when conversion has been finished. */
ISR(ADC_vect)
{
	sensor_value = ADCH;
	change_duty_led_x(sensor_value);
	
	/* Start the conversion again. */
	ADCSRA |= (1 << ADSC);	
}