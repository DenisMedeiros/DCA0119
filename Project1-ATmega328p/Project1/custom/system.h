/*
 * system.h
 *
 * Created: 27/03/2017 23:09:30
 *  Author: Denis
 */ 

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <avr/io.h>

#define SYSTEM_MODE_1_TIME 180
#define SYSTEM_MODE_2_TIME 60
#define LIGHT_THRESHOLD 40
#define HIGH_LIGHT_WEIGHT 0.5

/* This function initializes everything in the system. */
void system_init(void);
/* Start the system. */
void system_start(void);
/* Stop the system. */
void system_stop(void);

/* This function changes the operation mode of the dryer. */
void system_change_mode(uint8_t mode);

uint8_t dryer_total_time();
uint8_t dryer_value(uint8_t time);

/* Functions that define how the dryer works based on current time. */
uint8_t dryer_mode1(uint8_t time);
uint8_t dryer_mode2(uint8_t time);

#endif /* SYSTEM_H_ */