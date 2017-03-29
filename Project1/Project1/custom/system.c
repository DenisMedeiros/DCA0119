/*
 * system.c
 *
 * Created: 27/03/2017 23:09:42
 *  Author: Denis
 */ 

#include "usart.h"
#include "counters.h"
#include "digital.h"
#include "adc.h"

#include <avr/interrupt.h>
#include <math.h>

volatile circular_buffer USART_tx_buffer;
volatile circular_buffer USART_rx_buffer;
volatile uint32_t ticks = 0;
volatile char t_s[4], x_s[4], v_s[4];

volatile uint8_t system_running = 0;
volatile uint8_t sensor_value = 0;
volatile uint8_t total_time_running = 0;
volatile uint8_t system_mode = 0;

void system_init(void)
{
	/* Enable the Global Interrupt Enable flag, so that interrupts can be processed.	*/
	sei(); 
	
	/* Initializes the USART module. */
	USART_init(); 
	
	/* Initializes the counters module. */
	counters_init(); 
	
	/* Initializes digital input and output pins. */
	digital_init();
	
	/* Initializes analog to digital converter functionality. */
	adc_init(); 
}


void system_start(void)
{
	system_running = 1;
	
	/* Turn on LED 3 (system running). */
	PORTB |= (1 << PORTB0);
	
	/* Start the counters. */
	counters_start();
	
	/* Start the ADC. */
	adc_start();
	
	/* Inform to SCD that the system is running. */
	//buffer_add(&USART_tx_buffer, '+');
	//USART_enable_tx_interrupt();
}

void system_stop(void)
{
	system_running = 0;
	sensor_value = 0;
	total_time_running = 0;
	
	/* Turn off LED 3 (system stopped). */
	PORTB &= ~(1 << PORTB0);
	
	/* Turn of LED 1 and LED 2, as well as PWM generator. */
	change_duty_led_v(0);
	change_duty_led_x(0);
	
	/* Stop the counters. */
	counters_stop();
	
	/* Stop the ADC. */
	adc_stop();

	/* Inform to SCD that the system is stopped. */
	//buffer_add(&USART_tx_buffer, '-');
	//USART_enable_tx_interrupt();
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

uint8_t dryer_mode2(uint8_t time)
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