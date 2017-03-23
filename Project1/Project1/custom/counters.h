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

void counters_init(void);

#endif /* COUNTERS_H_ */