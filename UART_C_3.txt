/*Recibe un byte por uARST2 y lo regresa a la PC. BR=9600 1 bit de stop APB1=36MHz*/

#include "stm32f10x.h"

void usart2_sendByte(unsigned char c);
uint8_t usart2_recByte(void);

int main()
{
	RCC->APB1ENR |= (1<<17); 		//habilita reloj uart2
	RCC->APB2ENR |= 0xFC; 			//habilita reloj GPIOS
	
	//USART2 init.
	GPIOA->CRL = 0x44448B44; 		//RX2=PA3 con pull-up, TX2=PA2 con alt. func output
	GPIOA->ODR |= (1<<3); 			//pull-up PA3(RX2)
	USART2->CR1 = 0x200C;			//Frame 8 bits de dato, 1 bit Stop, sin bit paridad
	USART2->BRR = 3750; 			//36MHz/9600 = 3750	
	
	while(1)
	{	
		uint8_t receivedByte = usart2_recByte(); 	//Recibe Byte
		usart2_sendByte(receivedByte);			//Envía Byte recibido
	}
}

uint8_t usart2_recByte()
{
	while((USART2->SR&(1<<5)) == 0);	//Espera RXNE=1, Recepción completa
	
	return USART2->DR;	
}

void usart2_sendByte(unsigned char c)
{
	USART2->DR = c;
	
	while((USART2->SR&(1<<6)) == 0);	//Espera TC=1, Transmisión completa
	USART2->SR &= ~(1<<6);
}
