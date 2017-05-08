#!/usr/bin/env python
# -*- coding: utf-8 -*-

import threading
import time
import mraa


# Important constants

ADC_MAX_VALUE = 4096.0

# Configuration.

SENSOR_ADC_PIN = 0 # Used by the ADC (pin A0).
BUTTON_PULLUP_PIN = 4 # Used by the Hi-Z button.
LED_SENSOR_PWM_PIN = 5 
LED_DRYER_PWM_PIN = 6 
LED_SYSTEM_PIN = 7 

# Objects to access GPIO pins.

button = None
led_sensor = None
led_dryer = None
led_system = None
adc = None

threads = {}

# Important values.

sensor_value = 0
system_running = False

def button_pressed(pin):

    global system_runing
    print system_runing
    
    if system_runing:
        print '[button] stopping system...'
        stop()
        return
    
    print '[button] starting system...'
    start()
            
    
def sensor_worker():
    while True:
        time.sleep(1)
        print system_running
        if system_running:
            sensor_value = adc.read()
            print 'Value read by ADC = %f' %sensor_value

def control_worker():
    pass
    
def pwm_worker():
    pass
    
def network_worker():
    pass

def worker(num):
    """thread worker function"""
    print 'Worker: %s' % num
    return

def config():

    global button
    global led_sensor
    global led_dryer
    global led_system
    global adc

    # Configure the digital output for the LED 3.     
    led_system = mraa.Gpio(LED_SYSTEM_PIN) 
    led_system.dir(mraa.DIR_OUT)   

    # Configures the PWM generators (LEDs and dryer).
    led_sensor = mraa.Pwm(LED_SENSOR_PWM_PIN)
    #led_sensor.period_ms(100)
    #led_sensor.enable(True)

    led_dryer = mraa.Pwm(LED_DRYER_PWM_PIN)
    #led_sensor.period_ms(0)
    #led_sensor.enable(True)

    # Configures the Hi-Z button.
    button = mraa.Gpio(BUTTON_PULLUP_PIN) 
    button.dir(mraa.DIR_IN)        
    button.mode(mraa.MODE_PULLUP)
    button.isr(mraa.EDGE_FALLING, button_pressed, button)   

    # Configures the ADC.
    adc = mraa.Aio(SENSOR_ADC_PIN)
        

def start():
    global system_runing
    system_runing = True
    
    # Start ADC thread.
    threads['adc'] = threading.Thread(target=sensor_worker)
    threads['adc'].setDaemon(True)
    threads['adc'].start()
    
    led_system.write(1)
    
def stop():
    global system_runing
    system_runing = False
    
    threads['adc'].stop()
    
    led_system.write(0)
    
    
def start2():
    led_system.write(1)
     

if __name__ == "__main__":
    config()
    start()
    while 1:
        pass
 
