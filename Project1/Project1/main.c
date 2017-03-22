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
#include <stdlib.h>

circular_buffer USART_tx_buffer;
circular_buffer USART_rx_buffer;

int main(void)
{
	char t_s[8], x_s[8], v_s[8];
	uint8_t t = 0, v = 0, x = 0;
	
	sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed
	USART_init(); // Initializes the USART.

    while (1) {
		
		t++;
		v = t+2;
		x = t/2;
		
		itoa(t, t_s, 10);
		itoa(x, x_s, 10);
		itoa(v, v_s, 10);

		buffer_put_string(&USART_tx_buffer, "t=");
		buffer_put_string(&USART_tx_buffer, t_s);
		buffer_add(&USART_tx_buffer, ';');
		buffer_put_string(&USART_tx_buffer, "x=");
		buffer_put_string(&USART_tx_buffer, x_s);
		buffer_add(&USART_tx_buffer, ';');
		buffer_put_string(&USART_tx_buffer, "v=");
		buffer_put_string(&USART_tx_buffer, v_s);
		buffer_add(&USART_tx_buffer, ';');
		
		USART_enable_tx_interrupt();
		_delay_ms(1000);

	}
}

