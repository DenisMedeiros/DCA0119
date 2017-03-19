/*
 * usart.c
 *
 * Created: 18/03/2017 13:35:27
 *  Author: Denis
 */ 

#include "usart.h"
#include <avr/interrupt.h>

void USART_init(void) 
{
	/* Set baud rate */
	UBRR0H = (unsigned char) (BAUD_PRESCALLER >> 8);
	UBRR0L = (unsigned char) (BAUD_PRESCALLER);
	
	/* Enable receiver and transmitter */
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	
	/* Enable interruptions for reception */
	UCSR0B |= (1 << RXCIE0) | (1 << UDRIE0);
	
	/* Set frame format: 8 bits for data, 1 bit for stop bit */
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
	
}

void USART_send_byte(unsigned char data)
{
	/* Wait for empty transmit buffer */
	while(!(UCSR0A & (1 << UDRE0)));
	/* Put data into buffer, then sends the data */
	UDR0 = data;	
}

void USART_send_string(char* string){
	while(*string != '\0')
	{
		USART_send_byte(*string);
		string++;
	}
}

unsigned char USART_receive_byte(void)
{
	/* Wait for data to be received */
	while (!(UCSR0A & (1 << RXC0)));
	
	/* Get and return received data from buffer */
	return UDR0;
}


unsigned char buffer_add(circular_buffer* buffer, unsigned char c)
{
	uint8_t next_head_pos = (buffer->head_pos + 1) % BUFFER_SIZE;
	if (next_head_pos != buffer->tail_pos) {
		/* There is a room in the buffer */
		buffer->data[buffer->head_pos] = c;
		buffer->head_pos = next_head_pos;
		buffer->size++;
		return c;
	 } 
	 else 
	 {
		/* There is no room left in the buffer */
		return 255;
	 }
}

unsigned char buffer_remove(circular_buffer* buffer)
{
	char c;
	if (buffer->head_pos != buffer->tail_pos) {
		c = buffer->data[buffer->tail_pos];
		buffer->tail_pos = (buffer->tail_pos + 1) % BUFFER_SIZE;
		buffer->size--;
		return c;
	} 
	else 
	{
		return 255;
	}
}

unsigned char buffer_put_string(circular_buffer* buffer, unsigned char* string)
{
	while(*string != 0x00)
	{
		if(buffer_add(buffer, *string) == 255)
		{
			return 255;
		};
		string++;
	}
	
	return 0;
}

void USART_enable_tx_interrupt(void)
{
	 UCSR0B |= (1 << UDRIE0);
}


/* Interrupts handlers */
ISR(USART_RX_vect)
{
	unsigned char c = UDR0;
	buffer_add(&USART_rx_buffer, c);
	UDR0 = '0' + USART_rx_buffer.size;
}

ISR(USART_UDRE_vect)
{
	unsigned char c = buffer_remove(&USART_tx_buffer);
	if(c == 255) {
		/* Disable interrupt */
		UCSR0B &= ~(1 << UDRIE0);
		return;
	}
	UDR0 = c;	
}