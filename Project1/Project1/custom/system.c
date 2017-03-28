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

#include <avr/io.h>
#include <avr/interrupt.h>

volatile circular_buffer USART_tx_buffer;
volatile circular_buffer USART_rx_buffer;
volatile uint32_t ticks = 0;
volatile char t_s[8], x_s[8], v_s[8];
volatile uint8_t t = 0, v = 0, x = 0;
volatile uint8_t system_running = 0;
volatile uint8_t sensor_value = 0;
volatile uint8_t total_time_running = 0;

void init(void)
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