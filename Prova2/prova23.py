#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import mraa

'''
Pinagem do SPI no Galileo:
  CS = pino 10
  MOSI = 11
  MISO = 12
  SCLK = 13
'''


# Inicializa o SPI.
spi = mraa.Spi(0)
spi.mode(mraa.SPI_MODE0)
spi.frequency(2000000)
spi.lsbmode(False)


# Configure os seletores do escravo (é habilitado em nível baixo).

ss0 = mraa.Gpio(2)
ss0.dir(mraa.DIR_OUT)

ss1 = mraa.Gpio(3)
ss1.dir(mraa.DIR_OUT)

while 1:

    # Habilita o escravo 0. 
    ss0.write(0)  # Nível 0 é quem habilita.
    ss1.write(1) 
  
    # Envia o byte 0xC4 e recebe algum retorno do escravo 0.
    retorno_escravo0 = spi.writeByte(0xC4) 
    print retorno_escravo0

    # Envia o byte 0xD2 e recebe algum retorno do escravo 0.
    retorno_escravo0 = spi.writeByte(0xD2) 
    print retorno_escravo0

    # Habilita o escravo 1.
    ss0.write(1)  
    ss1.write(0) # Nível 0 é quem habilita.

    # Envia o byte 0xA1 e recebe algum retorno do escravo 1.
    retorno_escravo1 = spi.writeByte(0xA1) 
    print retorno_escravo1

    # Envia o byte 0xF4 e recebe algum retorno do escravo 1.
    retorno_escravo1 = spi.writeByte(0xF4) 
    print retorno_escravo1

