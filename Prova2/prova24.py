#!/usr/bin/env python
# -*- coding: utf-8 -*-

import mraa
import sys

'''
Pinagem:
GPIO0 - UART0_RXD
GPIO1 - UART0_TXD
'''

# Configura os parâmetros do UART.
uart = mraa.Uart(0)
uart.setBaudRate(9600)
uart.setMode(8, mraa.UART_PARITY_NONE, 1)
uart.setFlowcontrol(False, False)

while 1:
    # Envia a mensagem 'teste' via UART.
    msg_bytes = bytearray("teste", "ascii")
    uart.write(msg_bytes)
    uart.flush()

    # Aguarda o recebimento de algum dado.
    while not uart.dataAvailable():
        pass
        
    # Lê 1 byte do buffer.
    dado = uart.readStr(1)
    print dado



