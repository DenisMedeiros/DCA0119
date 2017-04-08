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

#define SYSTEM_MODE_1_TIME 180
#define SYSTEM_MODE_2_TIME 60

/* Buffer for USART transmission (sending). */
volatile circular_buffer USART_tx_buffer;
/* Buffer for USART transmission (receiving). */
volatile circular_buffer USART_rx_buffer;
/* Number of overflows of counter 0 (used to count seconds). */
volatile uint32_t ticks = 0;
/* Used to sent data to SCD by USART. */
volatile char t_s[4], x_s[4], v_s[4];

/* Saves the state of the system (0 = stopped; 1 = running). */
volatile uint8_t system_running = 0;
/* Current value from sensor (comes from ADC; can be from 0 to 255). */
volatile uint8_t sensor_value = 0;
volatile uint8_t total_time_running = 0;
volatile uint8_t system_mode = 1;

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

/* Start the system. */
void system_start(void)
{
	system_running = 1;
	
	/* Turn on LED 3 (system running). */
	PORTB |= (1 << PORTB0);
	
	/* Start the counters. */
	counters_start();
	
	/* Start the ADC. */
	adc_start();
}

/* Stop the system. */
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
}

/* Define how the dryer is going to work (default mode, needs 3 minutes). */
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

/* Define how the dryer is going to work (fast mode, needs 1 minute). */
uint8_t dryer_mode2(uint8_t time)
{
	float result_float = 0;
	uint8_t result_int = 0;
	
	if(time <= 10)
	{
		result_int = 10*time;
	}
	else if(time > 10 && time < 50) 
	{
		result_int = 100;
	}
	else 
	{
		result_float = -10 * time + 600;
		result_int = roundf(result_float);
	}
	
	return result_int;
}

/* This function changes the operation mode of the dryer. */
void system_change_mode(uint8_t mode)
{
	if (system_running)
	{
		system_stop();
	}
	
	system_mode = mode;
}

/* Return the current value of the PWN FAN based on the mode. */
uint8_t dryer_value(uint8_t time)
{
	if(system_mode == 1)
	{
		return dryer_mode1(time);
	}
	else if(system_mode == 2)
	{
		return dryer_mode2(time);
	}
	else
	{
		return 0;
	}
}

/* Return the total time that the system must run depending on the mode. */
uint8_t dryer_total_time()
{
	if (system_mode == 1)
	{
		return SYSTEM_MODE_1_TIME;
	}
	else
	{
		return SYSTEM_MODE_2_TIME;
	}
}