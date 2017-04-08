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
extern volatile char t_s[4], x_s[4], v_s[4];
extern volatile uint8_t sensor_value;
extern volatile uint8_t system_running;
extern volatile uint8_t total_time_running;

/* Configure the counters. */
void counters_init(void);
/* Start the counters. */
void counters_stop(void);
/* Stop the counters. */
void counters_start(void);

/* Change the duty cycle of the LED 3 (sensor). */
void change_duty_led_x(uint8_t duty);
/* Change the duty cycle of the LED 2 (PWM to FAN). */
void change_duty_led_v(uint8_t duty);

#endif /* COUNTERS_H_ */