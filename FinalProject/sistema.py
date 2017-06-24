#!/usr/bin/env python
# -*- coding: utf-8 -*-

import threading
import time
import socket
import mraa

''' ********** Configuration ********** '''

HOT_LED = 5 # Red LED
WARM_LED = 6 # Yellow LED  
COLD_LED = 7 # Blue LED
BUZZER_PIN = 10

SENSOR_ADC_PIN = 0 # Used by the ADC (pin A0).
BUTTON_PULLUP_PIN = 4 # Used by the Hi-Z button.

COLD_TEMPERATURE = 23
WARM_TEMPERATURE = 25
HOT_TEMPERATURE = 27
SERVER = 'www.dimap.ufrn.br/~denis/monitor/'

''' ********** End of configuration ********** '''

# Handles button interruption on falling edge.   
def button_pressed(pin):
    time.sleep(0.2) # debounce delay
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
            
           # Read current temperature. 
           system.read_sensor()
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
            
           if self.stopped():
                exit()  
            
           system.calculate_pwm_dryer()
           system.send_info_to_sdc()
           system.advance_time()
           time.sleep(1)
           

''' 
Function that controls the PWM buzzer signal.
'''          
class BuzzerThread(threading.Thread):   
                         
    def __init__(self):
        super(BuzzerThread, self).__init__()
        self._stop = threading.Event()

    def stop(self):
        self._stop.set()

    def stopped(self):
        return self._stop.isSet()
        
    def run(self):   
        while True:
            if self.stopped():
                exit()  
            system.buzzer.write(0)
            time.sleep(0.5)
            system.buzzer.write(1)
            time.sleep(0.5)

''' 
Function that controls the PWM hot LED signal.
'''          
class HotLEDThread(threading.Thread):   
                         
    def __init__(self):
        super(HotLEDThread, self).__init__()
        self._stop = threading.Event()

    def stop(self):
        self._stop.set()

    def stopped(self):
        return self._stop.isSet()
        
    def run(self):   
        while True:
            if self.stopped():
                exit()  
            system.hot_led.write(0)
            time.sleep(0.5)
            system.hot_led.write(1)
            time.sleep(0.5)         


class System:
      
    def __init__(self):
    
        # Important variables.
        self.threads = {}
        self.running = False
        self.sensor_value = 0
        self.seconds = 0
        self.pwm_hot_led = 0
        
        # Configure the digital outputs for cold and warm led.
        self.cold_led = mraa.Gpio(COLD_LED)
        self.warm_led = mraa.Gpio(WARM_LED) 
        self.hot_led = mraa.Gpio(HOT_LED)
        self.buzzer = mraa.Gpio(BUZZER_PIN)

        self.cold_led.dir(mraa.DIR_OUT)   
        self.warm_led.dir(mraa.DIR_OUT) 
        self.hot_led.dir(mraa.DIR_OUT) 
        self.buzzer.dir(mraa.DIR_OUT) 
        
        self.cold_led.write(0)
        self.warm_led.write(0)
        self.hot_led.write(0)
        self.buzzer.write(1)

        # Configures the Hi-Z button.
        self.button = mraa.Gpio(BUTTON_PULLUP_PIN) 
        self.button.dir(mraa.DIR_IN)        
        self.button.mode(mraa.MODE_PULLUP)
        self.button.isr(mraa.EDGE_FALLING, button_pressed, self.button)   

        # Configures the ADC.
        self.adc = mraa.Aio(SENSOR_ADC_PIN)

    def read_sensor(self):
        self.sensor_value = int(100 * (self.adc.read() / 1024.0))
        print self.sensor_value

    def adc_start(self):
        # Start ADC thread.
        self.threads['adc'] = ADCThread()
        self.threads['adc'].setDaemon(True)
        self.threads['adc'].start()
        
    def adc_stop(self):
        self.threads['adc'].stop()

    def buzzer_start(self):
        self.threads['buzzer'] = BuzzerThread()
        self.threads['buzzer'].setDaemon(True)
        self.threads['buzzer'].start()
  
    def buzzer_stop(self):
        self.threads['buzzer'].stop()
        self.buzzer.write(1)
    
    def hot_led_start(self): 
        self.threads['hot_led'] = HotLEDThread()
        self.threads['hot_led'].setDaemon(True)
        self.threads['hot_led'].start()

    def hot_led_stop(self): 
        self.threads['hot_led'].stop()
        self.hot_led.write(0)
            
    def control_start(self):
        self.threads['control'] = ControlThread()
        self.threads['control'].setDaemon(True)
        self.threads['control'].start()
        
    def control_stop(self):
        self.threads['control'].stop()
      
    def network_stop(self):
        self.threads['network'].stop()
   

    def start(self):
        self.running = True
        self.adc_start()
        self.buzzer_start()
        self.hot_led_start()

        #self.pwm_start()
        #self.control_start()
        #self.network_start()

    def stop(self):

        print "system stop"
        self.running = False
        self.sensor_value = 0
        self.adc_stop()
        self.buzzer_stop()
        self.hot_led_stop()


        #self.pwm_stop()
        #self.control_stop()



if __name__ == "__main__":
    try:
        system = System()
        while True: pass
    except KeyboardInterrupt:
        system.stop()
 