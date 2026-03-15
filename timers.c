#include "stm32f10x.h"
#include <stdio.h>

void delay_ms(uint16_t t);
void usart1_sendByte(unsigned char c);
void usart1_sendString(const char *str);
void delay(void);

int main()
{
	uint32_t tiempo_inicio = 0;
    uint32_t tiempo_final = 0;
    uint32_t tiempo_transcurrido_ms = 0;
    float tiempo_transcurrido_s = 0.0f;
    
	RCC->APB2ENR |= 0xFC | (1<<14); // Habilitar reloj de GPIOS y UART1
    
    // USART1 init.
    GPIOA->ODR |= (1<<10);    // pull-up en PA10
    GPIOA->CRH = 0x444448B4;    // RX1=PcooA10 con pull-up, TX1=PA9 con alt. func output
    USART1->CR1 = 0x200C;    // Habilita UART y por defecto frame de 8 bits de dato, 1 bit de parada, sin bit de paridad
    USART1->BRR = 7500;    // 72MHz/9600bps = 7500
    
    GPIOB->CRL = 0x84484444; // PB4-PB7 como entradas
		GPIOB->ODR &= ~(1 << 4);
		GPIOB->ODR &= ~(1 << 7);
    
    // Inicialización del temporizador TIM2
    RCC->APB1ENR |= (1<<0); // Habilitar reloj de TIM2
    TIM2->PSC = 7200-1; // PSC= 7,199; Frec=72M/PSC
    TIM2->ARR = 10000-1; // Conteo
    TIM2->SR = 0; // Borrar bandera UIF

    while(1)
    {  
			usart1_sendString("\n\nPresiona 4 para comenzar el conteo\n");
        while ((GPIOB->IDR & (1 << 4)) != 0) {
            delay_ms(100);
        }

        tiempo_inicio = TIM2->CNT;
        usart1_sendString("\nPresiona 7 para detener el conteo\n");

        while ((GPIOB->IDR & (1 << 7)) == 0) {
            delay_ms(100);
        }

        tiempo_final = TIM2->CNT;
        tiempo_transcurrido_ms = tiempo_final - tiempo_inicio;
        tiempo_transcurrido_s = (float)tiempo_transcurrido_ms / 1000.0f;

        usart1_sendString("\nDejo de contar");

        // Imprime el tiempo transcurrido en segundos por UART
        char buffer[50];
        sprintf(buffer, "\nTiempo transcurrido: %.2f segundos", tiempo_transcurrido_s);
        usart1_sendString(buffer);
					}
    }



void usart1_sendByte(unsigned char c)
{
    USART1->DR = c;
    
    while((USART1->SR&(1<<6)) == 0); // Espera TC=1, Transmisión completa
    USART1->SR &= ~(1<<6);        // Baja bandera a 0   
}
void usart1_sendString(const char *str) {
    while (*str) {
        usart1_sendByte(*str++);
    }
}

void delay_ms(uint16_t t)
{
    volatile unsigned long l = 0;
    for(uint16_t i = 0; i < t; i++)
        for(l = 0; l < 6000; l++)
        {
        }
}

void delay()
{
	TIM2->PSC = 7200-1;		//PSC= 7,199; Frec=72M/PSC (Prescaler)
	TIM2->ARR = 20000-1;		//Conteo 
	//(CNT cambia a una frecuencia de 10kHz entonces se hace 20000 veces para UIF suceda cada 2Hz o cada 2seg)
	TIM2->SR = 0;			//Borrar bandera UIF (candera en 0)
	TIM2->CR1 = 1;			//Conteo hacia arriba con relojj de 72000
	while((TIM2->SR & 1) == 0);	//Esperar hasta UIF =1 (cuando se levanta la bandera)
	TIM2->CR1 = 0;			//Detener conteo
}