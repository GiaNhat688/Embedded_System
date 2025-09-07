#include "stm32f10x.h"

void GPIO_Config(){
	RCC->APB2ENR |= (1<<2) | (1<<3);
    GPIOA->CRL &= ~(0xF<<8);   
    GPIOA->CRL |=  (0x2<<8);
	
	GPIOB->CRL &= ~(0xF<<0);  
    GPIOB->CRL |=  (0x2<<0);
}

void delay_ms(unsigned int t) {
    for (unsigned int i = 0; i < t * 8000; i++) {
    }
}

int main(void) {
	GPIO_Config();
	
    while (1) {
        GPIOA->ODR |= (1<<2);
        GPIOB->ODR |= (1<<0);
        delay_ms(1000);

        GPIOA->ODR &= ~(1<<2);
        GPIOB->ODR &= ~(1<<0);
        delay_ms(1000);
    }
}
