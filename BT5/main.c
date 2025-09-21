
#include "stm32f10x.h"

void TIM2_Init(void){
	RCC->APB1ENR |= 1 << 0;
	
	TIM2->PSC = 7200-1;
	TIM2->ARR = 10;
	TIM2->CR1 |= 1 << 0;
}

void delay_ms(uint32_t t){
	for(uint32_t i=0; i<t; i++){
			TIM2->CNT=0;
			while(!(TIM2->SR & 1));
			TIM2->SR &= ~(1<<0);
	}
}

void USART2_Init(void){
	RCC->APB1ENR |= 1 << 17;
	RCC->APB2ENR |= 1 << 2;
	
	GPIOA->CRL &= ~(1 << 8);
	GPIOA->CRL |= 0x0B << 8;
	
	GPIOA->CRL &= ~(1 << 12);
	GPIOA->CRL |= 0x0B << 12;

	USART2->BRR = 0xEA6; 
	
	USART2->CR1 |= 1 << 13;
	USART2->CR1 |= 1 << 2;
	USART2->CR1 |= 1 << 3;
}

void USART2_SendChar(char c){
		while(!(USART2->SR & (1 << 7)));
		USART2->DR = c;
}

void USART2_SendString(char *str){
	while(*str){
		USART2_SendChar(*str++);
	}
}

int main(void){
	USART2_Init();
	TIM2_Init();
	
	while(1){
		USART2_SendString("Hello from STM32!\r\n");
		delay_ms(1000);
	}
}
