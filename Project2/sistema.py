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

class ADCThread(threading.Thread):

    def __init__(self):
        super(ADCThread, self).__init__()
        self._stop = threading.Event()

    def stop(self):
        self._stop.set()

    def stopped(self):
        return self._stop.isSet()
        
    def run(self):    
        while True:
            sensor_value = system.get_adc_value()
            print 'Value read by ADC = %f' %sensor_value
            time.sleep(1)
            if self.stopped():
                exit()
         
     
def button_pressed(pin):
    if system.running:
        system.stop()
        return
    
    system.start()          
                
class System:
  
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
        
    def get_adc_value(self):
        return self.adc.read()

    def __init__(self):
    
        # Important variables.
        self.threads = {}
        self.running = False
        self.sensor_value = 0
        
        # Configure the digital output for the LED 3.     
        self.led_system = mraa.Gpio(LED_SYSTEM_PIN) 
        self.led_system.dir(mraa.DIR_OUT)   

        # Configures the PWM generators (LEDs and dryer).
        self.led_sensor = mraa.Pwm(LED_SENSOR_PWM_PIN)
        #led_sensor.period_ms(100)
        #led_sensor.enable(True)

        self.led_dryer = mraa.Pwm(LED_DRYER_PWM_PIN)
        #led_sensor.period_ms(0)
        #led_sensor.enable(True)

        # Configures the Hi-Z button.
        self.button = mraa.Gpio(BUTTON_PULLUP_PIN) 
        self.button.dir(mraa.DIR_IN)        
        self.button.mode(mraa.MODE_PULLUP)
        self.button.isr(mraa.EDGE_FALLING, button_pressed, self.button)   

        # Configures the ADC.
        self.adc = mraa.Aio(SENSOR_ADC_PIN)
        

    def start(self):
        print '[info] Starting the system...'
        self.running = True
        
        # Start ADC thread.
        self.threads['adc'] = ADCThread()
        self.threads['adc'].setDaemon(True)
        self.threads['adc'].start()
        
        self.led_system.write(1)
    
    def stop(self):
        print '[info] Stopping the system...'
        self.running = False
        
        # Stop ADC thread.
        self.threads['adc'].stop()
        self.led_system.write(0)
    
       

if __name__ == "__main__":
    system = System()
    system.start()
    while True: pass
 
