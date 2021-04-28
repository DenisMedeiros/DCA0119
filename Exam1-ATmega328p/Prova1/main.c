/*
 * Prova1.c
 *
 * Created: 01/05/2017 23:39:03
 * Author : Denis
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

/* Vari�veis globais vol�teis para evitar otimiza��o do compilador. */
volatile uint16_t sensor_A1 = 0;
volatile uint16_t sensor_A2 = 200;
volatile uint16_t sensor_umidade = 0;

int main(void)
{
    
    /* Configura��o das portas de sa�da. 
     * PB1 = S1, PB2 = S2, PD0 = LED L1, PD1 = LED L2, PD2 = LED L3.
     */
     
    /* Portas PB1 e PB2 como sa�da (para PWM). */
    DDRB |= (1 << DDB1) | (1 << DDB2); 
    /* Portas PD0, PD1 e PD2 como sa�das (LEDs). */
    DDRD |= (1 << DDD0) | (1 << DDD1) | (1 << DDD2); 
    
    /* Configura��o das portas de entrada.
     * PC0 = A1 (Tin), PC1 = A2 (Tout), PC2 = umidade, 
     * PB0 = chave Hi-Z (com pull-up).
     */
    PORTB = (1 << PORTB0);  /* Porta PB0 como entrada com pull-up. */
    
    /* Configura��o do ADC. */
    ADMUX |= (1 << REFS0); /* ADC utilizando a refer�ncia interna (Vcc).*/
    
    /* Habilita o ADC e configura e o prescaller para 128 (125 KHz) */
    ADCSRA |= (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
    
    /* Configura��o do contador 0, para realizar a leitura dos sensores. */
    TCCR0A |= (1 << COM0A1); /* Configura o contador 0 no modo normal. */
    
    /* Ativa a interrup��o de overflow para o contador 0. */
    TIMSK0 |= (1 <<  TOIE0);

    /* Configura o prescaller para 1024 e inicia o contador. */
    TCCR0B |= (1 << CS00) | (1 << CS02);

    /* Configura��o do contador 1, como gerador de PWM. 
     * OCR1A = S1 (Q(t)), OCR1B = S2 ((p(t)).
     */
    /* Configura o contador 1 como Fast-PWM, com 10 bits de resolu��o. */
    TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10) | (1 << WGM11);
    
    /* Configura o prescaller para 1024 (15,625 KHz). */
    TCCR1B |= (1 << WGM12) | (1 << CS10) | (1 << CS12);
    
    /* Inicializa os valores do duty cycle como 0 inicialmente.  */
    OCR1A = 0; /* S1 = D1 = Q(t) */
    OCR1B = 0; /* S2 = D2 = P(t) */
    
    while (1) 
    {
        /* Verifica se a chave CH1 (PB0) est� ligada (Hi-Z = 1 = ligado). */
        while(!(PINB & 0b00000001)) 
        {
            /* Enquanto a chave estiver desligada, 
             * aguarde aqui e mantenha os LEDs apagados. 
             */
            PORTD &= ~(1 << PORTD0); /* Apaga o LED L1 (PD0). */
            PORTD &= ~(1 << PORTD1); /* Apaga o LED L2 (PD1). */
            PORTD &= ~(1 << PORTD2); /* Apaga o LED L3 (PD2). */
        }
        
        /* Verifica se a temperatura de entrada � menor que 20�C. 
         * Se sim, acende o LED e reinicie o loop. 
         */
        if(sensor_A1 < 102)
        {
            PORTD |= (1 << PORTD0); /* Liga o LED L1 (PD0). */
            continue;
        }

        /* Se chegou aqui, a temperatura � maior que 20�C. */
        /* Apaga (ou mantenha apagado) o LED L1 (PD0). */
        PORTD &= ~(1 << PORTD0); 
        
        /* Verifica se a temperatura de sa�da � maior que 120�C. 
         * Se sim, acende o LED e reinicie o loop. 
         */
        if(sensor_A2 > 614)
        {
            PORTD |= (1 << PORTD1); /* Liga o LED L2 (PD1). */
            continue;
        }
        
        /* Se chegou aqui, a temperatura � menor que 120�C, 
         * al�m de ser maior que 20�C. 
         */  
         /* Apaga (ou mantenha apagado) o LED L2 (PD1). */
        PORTD &= ~(1 << PORTD1); 
        
        /* A partir daqui, segue a situa��o com o sistema 
         * operando normalmente. 
         */
     
        PORTD |= (1 << PORTD2); /* Acende o LED L3 (PD2). */
        
         /* Umidade em 100%. */
        if(sensor_umidade > 510)
        {
            OCR1A = 255; /* Q(t) em 25 %. */
            OCR1B = 1023; /* P(t) em 100 %. */
        }
        /* Umidade entre 50% e 100%. */
        else if (sensor_umidade > 255 && sensor_umidade <= 510) 
        {
            OCR1A = 511; /* Q(t) em 50 %. */
            OCR1B = 511; /* P(t) em 50 %. */
        }
        /* Umidade entre 25% e 50%. */
        else if (sensor_umidade > 127 && sensor_umidade <= 255) 
        {
            OCR1A = 1023; /* Q(t) em 100 %. */
            OCR1B = 255; /* P(t) em 25 %. */
        }
        /* Umidade abaixo de 25%. */
        else
        {
            OCR1A = 0; /* Q(t) em 0 %. */
            OCR1B = 0; /* P(t) em 0 %. */
        }
    }
}

/* Interrup��o que ocorre para o contador 0 (aproximadamente a cada 16 ms).  
 * Aqui, os valores dos sensores s�o lidos a partir do ADC.
 */
ISR (TIMER0_COMPA_vect)  
{
    /* Faz a leitura dos 3 sensores. */
    
    /* L� o sensor A1. */
    ADMUX = (ADMUX & 0b11110000) | (1 << MUX0); /* Seleciona o ADC0. */
    ADCSRA |= (1 << ADSC); /* Inicia a convers�o. */
    while(!(ADCSRA & 0b00010000)); /* Aguarda o fim da convers�o. */
    sensor_A1 = ADC;
    
    /* L� o sensor A2. */
    ADMUX = (ADMUX & 0b11110000) | (1 << MUX1); /* Seleciona o ADC1. */
    ADCSRA |= (1 << ADSC); /* Inicia a convers�o. */
    while(!(ADCSRA & 0b00010000)); /* Aguarda o fim da convers�o. */
    sensor_A2 = ADC;
    
    /* L� o sensor de umidade. */
    ADMUX = (ADMUX & 0b11110000) | (1 << MUX2); /* Seleciona o ADC2. */
    ADCSRA |= (1 << ADSC); /* Inicia a convers�o. */
    while(!(ADCSRA & 0b00010000)); /* Aguarda o fim da convers�o. */
    sensor_umidade = ADC;
}