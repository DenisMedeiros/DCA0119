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

LED_SYSTEM_PIN = 7 
LED_DRYER_PWM_PIN = 5 
LED_SENSOR_PWM_PIN = 6

# Handles button interruption on falling edge.   
def button_pressed(pin):
    if system.running:
        print '[button] The system has been stopped.' 
        system.stop()
        return
    print '[button] The system has been started.' 
    system.start()          



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
           if self.stopped():
                exit()  
           system.update_sensor_value()
           time.sleep(1)

                  
class NetworkThread(threading.Thread):

    def __init__(self):
        super(NetworkThread, self).__init__()
        self._stop = threading.Event()

    def stop(self):
        self._stop.set()

    def stopped(self):
        return self._stop.isSet()
        
    def run(self):    
        while True:
           if self.stopped():
                exit()  
           system.update_sensor_value()
           time.sleep(1)  

''' 
Function that controls the PWM signal.
'''          
class PWMThread(threading.Thread):   
                         
    def __init__(self):
        super(PWMThread, self).__init__()
        self._stop = threading.Event()

    def stop(self):
        self._stop.set()

    def stopped(self):
        return self._stop.isSet()
        
    def run(self):    
        while True:
            if self.stopped():
                exit()
            
            sensor_value = system.sensor_value
            
            
            time.sleep(1) 
            
                  
         
         
''' 
Function that controls the curve signal.
'''          
class ControlThread(threading.Thread):   
                         
    def __init__(self):
        super(ControlThread, self).__init__()
        self._stop = threading.Event()

    def stop(self):
        self._stop.set()

    def stopped(self):
        return self._stop.isSet()
        
    def run(self):    
        current_time = 0
        while True:
            
           if self.stopped() or current_time > 180:
                system.stop()
                exit()  
            
           system.spend_time()
           if system.mode == 1:
               pwm_duty = system.dryer_mode1()
               system.set_sensor_pwm_duty(system.sensor_value/100.0)
               system.set_dryer_pwm_duty(pwm_duty/100.0)
           elif system.mode == 2:
               pwm_duty = system.dryer_mode2()
               
           
           time.sleep(1)  
                
                
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
        
    def update_sensor_value(self):
        self.sensor_value = int(100 * (self.adc.read() / 1024.0))
        print '[adc] Current value: %d %%' %self.sensor_value 
    
    ''' duty must be between 0 and 1. '''
    def set_sensor_pwm_duty(self, duty):
        print '[pwm] Sensor PWM: %d' %duty
        self.led_sensor.write(duty)
    
    ''' duty must be between 0 and 1. '''  
    def set_dryer_pwm_duty(self, duty):
        print '[pwm] Dryer PWM: %d' %duty
        self.led_dryer.write(duty)

    def __init__(self):
    
        # Important variables.
        self.threads = {}
        self.running = False
        self.sensor_value = 0
        self.seconds = 0
        self.mode = 1
        
        # Configure the digital output for the LED 3.     
        self.led_system = mraa.Gpio(LED_SYSTEM_PIN) 
        self.led_system.dir(mraa.DIR_OUT)   

        # Configures the PWM generators (LEDs and dryer).
        self.led_sensor = mraa.Pwm(LED_SENSOR_PWM_PIN)
        self.led_sensor.period_ms(5)
        self.led_sensor.enable(True)

        self.led_dryer = mraa.Pwm(LED_DRYER_PWM_PIN)
        self.led_dryer.period_ms(5)
        self.led_dryer.enable(True)

        # Configures the Hi-Z button.
        self.button = mraa.Gpio(BUTTON_PULLUP_PIN) 
        self.button.dir(mraa.DIR_IN)        
        self.button.mode(mraa.MODE_PULLUP)
        self.button.isr(mraa.EDGE_FALLING, button_pressed, self.button)   

        # Configures the ADC.
        self.adc = mraa.Aio(SENSOR_ADC_PIN)
        
   
        

    def start(self):
        self.running = True
        
        # Start ADC thread.
        self.threads['adc'] = ADCThread()
        self.threads['adc'].setDaemon(True)
        self.threads['adc'].start()
        
        # Start the control thread.
        self.threads['control'] = ControlThread()
        self.threads['control'].setDaemon(True)
        self.threads['control'].start()
        
        self.led_system.write(1)

    
    def stop(self):
        self.running = False
        
        # Stop threads.
        self.threads['adc'].stop()
        self.threads['control'].stop()
        
        self.led_system.write(0)
    
    
    def spend_time(self):
        self.seconds += 1

     
    def dryer_mode1(self):
    
        if self.seconds < 30:
            result = self.seconds
        elif self.seconds > 30 and self.seconds <= 60:
            result = 30
        elif self.seconds > 60 and self.seconds <= 90:
            result = int(1.5 * self.seconds - 60)
        elif self.seconds > 90 and self.seconds <= 120:
            result = 75
        elif self.seconds > 120 and self.seconds <= 180:
            result = int(-1.25 * self.seconds + 225)
        else:
            result = 0
        return result

    def dryer_mode2(self):
    
        if self.seconds <= 10:
            result = 10 * self.seconds
        elif self.seconds > 10 and self.seconds < 50:
            result = 100;
        else:
            result = int(-10 * self.seconds + 600)    
        return result


if __name__ == "__main__":
    system = System()
    while True: pass
 
