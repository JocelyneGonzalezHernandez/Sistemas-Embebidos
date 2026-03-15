#include "stm32f10x.h"
#include <stdio.h>

void delay_ms(uint16_t t);
void usart2_sendByte(char * str);
void preguntas(int i);
void answer(int *pregunta, int *respuesta, char inciso);

int main()
{
	int pregunta = 1;
  int respuesta = 0;
	int i = 1;
	int lope = 0;
	char R_final[80];
	
	RCC->APB1ENR |= (1<<17); 	//prende reloj UART2
	RCC->APB2ENR |= 0xFC; 		//prende reloj GPIOS
	
	GPIOB->CRL = 0x44488888;  //habilita pull-down
	GPIOB->ODR &= ~(1<<1); 	 	//pull-down en PB1
	GPIOB->ODR &= ~(1<<0);		//pull-down en PB0
	GPIOB->CRH = 0x44488888; 
	GPIOB->ODR &= ~(1<<10);		//pull-down en PB10

	//USART2 init.
	GPIOA->CRL = 0x44448B44; //RX2=PA3 con pull-up, TX2=PA2 con alt. func output
	GPIOA->ODR |= (1<<3); 	 // pull-up en PA3(RX2)
	USART2->CR1 = 0x200C; 	 // Frame 8 bits de dato, 1 bit Stop, sin bit paridad
	USART2->BRR = 3750; 		 //36MHz/9600 = 3750	
	
	while(i<5)
	{		
		//Funcion imprimir preguntas
		preguntas(i);
		
		while(lope==0){
			if((GPIOB->IDR&(1<<1)) != 0) 			//Inciso a con el boton B1
			{
				answer(&pregunta, &respuesta, 'a');
				lope=1;
			}
			if((GPIOB->IDR&(1<<0)) != 0) 			//Inciso b con el boton B0
			{
				answer(&pregunta, &respuesta, 'b');
				lope=1;				
			}
			if((GPIOB->IDR&(1<<10)) != 0) 			//Inciso c con el boton B10
			{
				answer(&pregunta, &respuesta, 'c');
				lope=1;
			}	
		}
		lope=0;
		delay_ms(500);
		i++;
	}
	sprintf(R_final, "Tu puntaje es de: %d\n", respuesta);
	usart2_sendByte("\n\r"); 	//nueva línea
	usart2_sendByte(R_final);
	while(1){
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

void usart2_sendByte(char * str)
{
	unsigned char i = 0;

	while(str[i] != 0) 									//recorre string
	{
    USART2->DR = str[i];							//Manda la cadena
	  while((USART2->SR&(1<<6)) == 0);	//Espera TC=1, Transmision completa
	  USART2->SR &= ~(1<<6);
		i++;															//Aumenta el contador
	}
}

void preguntas(int i)
{		
	if (i == 1){
		//------------------------------------Pregunta 1-------------------------------------------------
		usart2_sendByte(" I) ÀEn quŽ a–o debuto Super Junior?");
		usart2_sendByte("\n\r"); 	//nueva línea
		usart2_sendByte("a) 2005");
		usart2_sendByte("\n\r"); 	//nueva línea
		usart2_sendByte("b) 2018");
		usart2_sendByte("\n\r"); 	//nueva línea
		usart2_sendByte("c) 2002");
	}
	if (i == 2){
		//------------------------------------Pregunta 2-------------------------------------------------
		usart2_sendByte("\n"); 	//nueva línea
		usart2_sendByte("\n"); 	//nueva línea
		usart2_sendByte("\r"); 	//comienzo de línea
		usart2_sendByte(" II) ÀLe caemos bien prof?");
		usart2_sendByte("\n\r"); 	//nueva línea
		usart2_sendByte("a) No");
		usart2_sendByte("\n\r"); 	//nueva línea
		usart2_sendByte("b) Si");
		usart2_sendByte("\n\r"); 	//nueva línea
		usart2_sendByte("c) Sin comentarios");
	}
	if (i == 3){
		//------------------------------------Pregunta 3-------------------------------------------------
		usart2_sendByte("\n"); 	//nueva línea
		usart2_sendByte("\n"); 	//nueva línea
		usart2_sendByte("\r"); 	//comienzo de línea
		usart2_sendByte(" III) ÀEn quŽ a–o debuto BTS?");
		usart2_sendByte("\n\r"); 	//nueva línea
		usart2_sendByte("a) 2010");
		usart2_sendByte("\n\r"); 	//nueva línea
		usart2_sendByte("b) 2016");
		usart2_sendByte("\n\r"); 	//nueva línea
		usart2_sendByte("c) 2013");
	}
	if (i == 4){
		//------------------------------------Pregunta 3-------------------------------------------------
		usart2_sendByte("\n"); 	//nueva línea
		usart2_sendByte("\n"); 	//nueva línea
		usart2_sendByte("\r"); 	//comienzo de línea
		usart2_sendByte(" IV) ÀVamos a pasar la materia?");
		usart2_sendByte("\n\r"); 	//nueva línea
		usart2_sendByte("a) Sí");
		usart2_sendByte("\n\r"); 	//nueva línea
		usart2_sendByte("b) Claramente");
		usart2_sendByte("\n\r"); 	//nueva línea
		usart2_sendByte("c) Sin dudas");
	}
}

void answer(int *pregunta, int *respuesta, char inciso)
{
	int P= *pregunta;
	
	if(P == 1){
		usart2_sendByte("\n"); 	//nueva linea
		usart2_sendByte("\n"); 	//nueva linea
		usart2_sendByte("\r"); 	//comienzo de linea
		usart2_sendByte("Tu respuesta para la pregunta 1 fue guardada");
		if(inciso == 'a'){
			(*respuesta)++;
		}
		(*pregunta)++;
	}
	if(P == 2){
		usart2_sendByte("\n"); 	//nueva linea
		usart2_sendByte("\r"); 	//comienzo de linea
		usart2_sendByte("Tu respuesta para la pregunta 2 fue guardada");
		if(inciso == 'b'){
			(*respuesta)++;
		}
		(*pregunta)++;
	}
	if(P == 3){
		usart2_sendByte("\n"); 	//nueva linea
		usart2_sendByte("\r"); 	//comienzo de linea
		usart2_sendByte("Tu respuesta para la pregunta 3 fue guardada");
		if(inciso == 'c'){
			(*respuesta)++;
		}
		(*pregunta)++;
	}
	if(P == 4){
		usart2_sendByte("\n"); 	//nueva linea
		usart2_sendByte("\r"); 	//comienzo de linea
		usart2_sendByte("Tu respuesta para la pregunta 4 fue guardada");
		if(inciso == 'b'){
			(*respuesta)++;
		}
		(*pregunta)++;
	}
}