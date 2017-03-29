/*
 * io.h
 *
 * Created: 27/03/2017 12:39:04
 *  Author: Denis
 */ 


#ifndef DIGITAL_H_
#define DIGITAL_H_

#include <avr/io.h>

extern volatile uint8_t system_running;

void digital_init(void);

#endif /* DIGITAL_H_ */