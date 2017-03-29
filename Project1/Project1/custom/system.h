/*
 * system.h
 *
 * Created: 27/03/2017 23:09:30
 *  Author: Denis
 */ 

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <avr/io.h>

/* This functions initializes everything in the system. */
void system_init(void);
/* Start the system. */
void system_start(void);
/* Stop the system. */
void system_stop(void);

void system_change_mode(uint8_t mode);

uint8_t dryer_mode1(uint8_t time);
uint8_t dryer_mode2(uint8_t time);

#endif /* SYSTEM_H_ */