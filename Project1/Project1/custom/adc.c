/*
 * adc.c
 *
 * Created: 27/03/2017 16:18:47
 *  Author: Denis
 */ 

#include "adc.h"
#include <avr/interrupt.h>

#include <stdlib.h>

void adc_init(void)
{
	/* Configure the ADC to use internal Vcc, 8-bit precision and ADC0 (default). */
	ADMUX |= (1 << REFS0) | (1 << ADLAR);
	
	/* Configure the prescaller to 128 (125 KHz) and enable the ADC. */
	ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);

	/* Configure to free running. */
	ADCSRB &= ~(1 << ADTS0) & ~(1 << ADTS1) & ~(1 << ADTS2);
}

void adc_start(void)
{
	ADCSRA |= (1 << ADEN);
	ADCSRA |= (1 << ADSC);
	
}

void adc_stop(void)
{
	ADCSRA &= ~(1 << ADEN);
	ADCSRA &= ~(1 << ADSC);
}

ISR(ADC_vect)
{
	int i = 0;
	char output[10];
	sensor_value = ADCH;
	
	itoa(sensor_value, output, 10);
	for(i = 0; i < 10; i++)
	{
		if (output[i] == 0) 
		{
			output[i] = '\n';
			output[i+1] = 0;
			break;
		}
	}
	
	//USART_send_string(output);
	
	/* Start the conversion again. */
	ADCSRA |= (1 << ADSC);
		
}