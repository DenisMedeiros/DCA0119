/*
 * counters.h
 *
 * Created: 22/03/2017 22:32:00
 *  Author: Denis
 */ 

#ifndef COUNTERS_H_
#define COUNTERS_H_

#include "usart.h"

extern volatile circular_buffer USART_tx_buffer;
extern volatile circular_buffer USART_rx_buffer;
extern volatile uint32_t ticks;

extern volatile char t_s[8], x_s[8], v_s[8];
extern volatile uint8_t t, v, x;

void counters_init(void);

#endif /* COUNTERS_H_ */