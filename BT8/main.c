#include "stm32f10x.h"
#include "stdio.h"

void gpio_config_PA1(void);
void usart1_config_PA9_PA10(void);
void tim2_config(void);
void adc1_config(void);

void delay_us(uint32_t t);

void usart1_sendchar(char c);
void usart1_sendstring(const char *str);

void gpio_config_PA1(){
	RCC->APB2ENR |= 1<<2;
	
	GPIOA->CRL &= ~(0xF<<4);
	GPIOA->CRL |= 0x0<<4;
}

void usart1_config_PA9_PA10(){
	RCC->APB2ENR |= 1<<14;
	
	GPIOA->CRH &= ~(0xF<<4);
	GPIOA->CRH |= 0xB<<4;
	
	GPIOA->CRH &= ~(0xF<<8);
	GPIOA->ODR |= 1<<10;
	GPIOA->CRH |= 0x8<<8;
	
	USART1->BRR = 0X1D4C;
	USART1->CR1 |= 1<<2 | 1<<3 | 1<<13;
}

void tim2_config(){
	RCC->APB1ENR |= 1<<0;
	
	TIM2->PSC = 72-1;
	TIM2->ARR = 0XFFFF;
	TIM2->CR1 |= 1<<0;
}

void delay_us(uint32_t t){
		TIM2->CNT=0;
		while(TIM2->CNT < t);
}

void usart1_sendchar(char c){
	while(!(USART1->SR & (1<<6)));
	USART1->DR = c;
}

void usart1_sendstring(const char *str){
	while(*str){
		usart1_sendchar(*str++);
	}
}

void adc1_config(){
	RCC->APB2ENR |= 1<<9;
	
	ADC1->CR2 |= 1<<0;
	ADC1->SMPR2 |= 7<<3; 
	
	delay_us(2);
	
	ADC1->SQR3 |= 1<<0; 
	ADC1->CR2 |= 1<<0;
	while(!(ADC1->SR & (1<<1)));
}

uint32_t adc1_read(){
	ADC1->CR2 |= 1<<0;
	while(!(ADC1->SR & (1<<1)));
	return ADC1->DR;
}

int main(){
	gpio_config_PA1();
	usart1_config_PA9_PA10();
	tim2_config();
	adc1_config();
	
	char buff[32];
	
	while(1){
		uint32_t adc = adc1_read();
		
		float tmp = (adc * 100.0f) / 4095.0f;
		
		sprintf(buff, "ADC = %d, TMP = %.2f\r\n", adc, tmp);
		
		usart1_sendstring(buff);
		delay_us(800); 
	}
}
