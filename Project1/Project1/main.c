/*
 * Project1.c
 *
 * Created: 18/03/2017 13:12:10
 * Author : Denis
 */ 

#include "custom/usart.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

circular_buffer USART_tx_buffer;
circular_buffer USART_rx_buffer;

unsigned char text[]  = "x=30.0;v=50.0";

int main(void)
{
	
	buffer_put_string(&USART_tx_buffer, text);
	
	sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed
	USART_init(); // Initializes the USART.
	
    while (1) {
		_delay_ms(1000);
		buffer_put_string(&USART_tx_buffer, text);
		USART_enable_tx_interrupt();
	}
}

