/*
 * usart.h
 *
 * Created: 18/03/2017 13:22:32
 *  Author: Denis
 */ 

#ifndef USART_H_
#define USART_H_

#define F_CPU 16000000UL /* CPU clock */
#define BAUD 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUD * 16UL))) - 1)    //The formula that does all the required maths


/* BUFFER_SIZE must be power of 2 */
#define BUFFER_SIZE   64

#include <avr/io.h>

typedef struct
{
    unsigned char data[BUFFER_SIZE];
	uint8_t size;
    volatile uint8_t head_pos;
    volatile uint8_t tail_pos;
} circular_buffer;

extern circular_buffer USART_tx_buffer;
extern circular_buffer USART_rx_buffer;

/* USART functions */
void USART_init(void);
void USART_send_byte(unsigned char data);
void USART_send_string(char* data);
unsigned char USART_receive_byte (void);
void USART_enable_tx_interrupt(void);

/* Circular buffer functions */
unsigned char buffer_add(circular_buffer* buffer, unsigned char c);
unsigned char buffer_put_string(circular_buffer* buffer, unsigned char* string);
unsigned char buffer_remove(circular_buffer* buffer);


#endif /* USART_H_ */