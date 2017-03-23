/*
 * Project1.c
 *
 * Created: 18/03/2017 13:12:10
 * Author : Denis
 */ 

#include "custom/usart.h"
#include "custom/counters.h"

#include <avr/io.h>
#include <avr/interrupt.h>

volatile circular_buffer USART_tx_buffer;
volatile circular_buffer USART_rx_buffer;
volatile uint32_t ticks = 0;

int main(void)
{

	sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed
	
	buffer_add(&USART_tx_buffer, '$');
	USART_init(); // Initializes the USART
	counters_init(); // Initializes the counters
	
    while (1) 
	{
	}
		
}