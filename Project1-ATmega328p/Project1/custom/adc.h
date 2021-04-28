/*
 * adc.h
 *
 * Created: 27/03/2017 16:18:17
 *  Author: Denis
 */ 
#include <avr/io.h>
#include "usart.h"

#ifndef ADC_H_
#define ADC_H_

extern volatile circular_buffer USART_tx_buffer;
extern volatile uint8_t system_running;
extern volatile uint8_t sensor_value;

/* Configure the ADC. */
void adc_init(void);
/* Start the ADC. */
void adc_start(void);
/* Stop the ADC. */
void adc_stop(void);

#endif /* ADC_H_ */