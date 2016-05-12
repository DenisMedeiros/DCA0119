#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import mraa
import random

# Pino onde será gerado o sinal PWM.
PWM_PIN = 5

# Definição das 5 entradas do ADC.
adc0 = mraa.Aio(0)
adc1 = mraa.Aio(1)
adc2 = mraa.Aio(2)
adc3 = mraa.Aio(3)
adc4 = mraa.Aio(4)

# Definição do gerador de PWM.
pwm = mraa.Pwm(PWM_PIN)
pwm.enable(True)
pwm.period_us(5000)




while 1:

    # Lê os valores do ADC.
    x0 = adc0.read()  
    x1 = adc1.read()
    x2 = adc2.read()
    x3 = adc3.read()
    x4 = adc4.read()

    # Gera os valores dos pesados de forma aleatória.
    w0 = random.random()
    w1 = random.random()
    w2 = random.random()
    w3 = random.random()
    w4 = random.random()

    # Calcula a saída.
    y = x0*w0 + x1*w1 + x2*w2 + x3*w3 + x4*w4

  
    # O ADC tem 10 bits de precisão, logo o maior valor posível de y é 4 * 1024 = 4096.
    y_final = y/4096.0

    # A entrada deve ser entre 0 e 1 (1 = 100%).
    pwm.write(y_final)   
    



