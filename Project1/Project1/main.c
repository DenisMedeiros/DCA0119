/*
 * Project1.c
 *
 * Created: 18/03/2017 13:12:10
 * Author : Denis
 */ 

#include "custom/usart.h"
#include "custom/counters.h"
#include "custom/digital.h"
#include "custom/adc.h"

#include <avr/io.h>
#include <avr/interrupt.h>

volatile circular_buffer USART_tx_buffer;
volatile circular_buffer USART_rx_buffer;
volatile uint32_t ticks = 0;

volatile char t_s[8], x_s[8], v_s[8];
volatile uint8_t t = 0, v = 0, x = 0;

volatile uint8_t system_running = 0;

volatile uint8_t sensor_value = 0;

int main(void)
{

	sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed
	
	USART_init(); // Initializes the USART
	counters_init(); // Initializes the counters
	digital_init(); // Initializes digital input and output
	adc_init(); // Initializes the ADC
	
    while (1) 
	{
	}
		
}