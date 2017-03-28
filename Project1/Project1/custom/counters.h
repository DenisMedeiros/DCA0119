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

extern volatile uint8_t sensor_value;

extern volatile uint8_t system_running;
extern volatile uint8_t total_time_running;

void counters_init(void);
void counters_stop(void);
void counters_start(void);

void change_duty_led1(uint8_t duty);
void change_duty_led2(uint8_t duty);

/* time must be in seconds. */
uint8_t dryer_mode1(uint8_t time);

#endif /* COUNTERS_H_ */