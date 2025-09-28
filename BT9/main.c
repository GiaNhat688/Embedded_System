#include "stm32f10x.h"
#include "stdio.h"

#define BUF_LEN 16
static uint16_t adc_buf[BUF_LEN];

void GPIO_Config_PA1(void);
void USART1_Config(void);
void TIM2_Config(void);
void DMA1_Channel1_Config(void);
void ADC1_Config(void);

void delay_us(uint32_t t);

void USART1_SendChar(char c);
void USART1_SendString(const char *str);

void GPIO_Config_PA1(){
	RCC->APB2ENR |= 1<<2;
	
	//analog mode
	GPIOA->CRL &= ~(0xF<<4);
	GPIOA->CRL |= 0x0<<4;
}

void TIM2_Config(){
	RCC->APB1ENR |= 1<<0;
	
	TIM2->PSC=72-1;
	TIM2->ARR=0xFFFF;
	TIM2->CR1 |= 1<<0;
}

void delay_us(uint32_t t){
	TIM2->CNT=0;
	while(TIM2->CNT < t);
}

void USART1_Config(){
	RCC->APB2ENR |= 1<<14;
	
	//pa9
	GPIOA->CRH &= ~(0xF<<4);
	GPIOA->CRH |= 0xB<<4;
	
	//pa10
	GPIOA->CRH &= ~(0xF<<8);
	GPIOA->ODR |= 1<<10;
	GPIOA->CRH |= 0x8<<8;
	
	//baud rate = 9600
	USART1->BRR = 0x1D4C;
	
	USART1->CR1 |= (1<<2) | (1<<3) | (1<<13);
}

void USART1_SendChar(char c){
	while(!(USART1->SR & (1<<6)));
	USART1->DR = c;
}

void USART1_SendString(const char *str){
	while(*str){
		USART1_SendChar(*str++);
	}
}

void ADC1_Config(){
	RCC->APB2ENR |= 1<<9; 
	RCC->CFGR |= (2<<14);
	
	ADC1->CR2 |= 1<<0;
	
	ADC1->SMPR2 &= ~(7<<3);
	ADC1->SMPR2 |= 7<<3;
	
	delay_us(2);
	
	ADC1->CR2 |= 1<<3;
	ADC1->CR2 |= 1<<1;
	ADC1->SQR3 |= 1<<0; 
	ADC1->CR2 |= 1<<8;

	ADC1->CR2 |= 1<<0;
}

void DMA1_Channel1_Config(){
	RCC->AHBENR |= 1<<0;
	
	DMA1_Channel1->CPAR = (uint32_t) &ADC1->DR; //peripheral register
	DMA1_Channel1->CMAR = (uint32_t) adc_buf;		//memory register
	DMA1_Channel1->CNDTR = BUF_LEN; //decrease
	
	DMA1_Channel1->CCR |= 1<<7;
	
	DMA1_Channel1->CCR |= 1<<8;
	DMA1_Channel1->CCR |= 1<<10;
	
	DMA1_Channel1->CCR |= (1<<5);
	
	DMA1_Channel1->CCR |= 1<<1;
	DMA1_Channel1->CCR |= 1<<0;
	
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
} 

void DMA1_Channel1_IRQHandler(void){
    if(DMA1->ISR & (1<<1)){
        DMA1->IFCR |= 1<<1;
        char buff[32];
				float Volt[BUF_LEN];
			
        for(int i=0; i<BUF_LEN; i++){
						Volt[i]=(adc_buf[i] * 3.3f) / 4095.0f; 
            sprintf(buff, "ADC[%d]=%u  Voltage[%d]=%.2f\r\n", i, adc_buf[i], i, Volt[i]);
            USART1_SendString(buff);
        }
    }
}

int main(){
	GPIO_Config_PA1();
	USART1_Config();
	TIM2_Config();
	ADC1_Config();
	DMA1_Channel1_Config();
	
	while(1){

	}
}
