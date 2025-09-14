#include "stm32f10x.h"

void TIM2_Init(void) {
    RCC->APB1ENR |= (1<<0);
    TIM2->PSC = 7200 - 1;
    TIM2->ARR = 10 - 1;
    TIM2->CR1 |= (1<<0);
}

void delay_ms(uint32_t t) {
    for(uint32_t i = 0; i < t; i++) {
        TIM2->CNT = 0;
        while((TIM2->SR & (1<<0)) == 0);
        TIM2->SR &= ~(1<<0);
    }
}

void EXTI0_IRQHandler(void) {
    if (EXTI->PR & (1<<0)) {
        EXTI->PR = (1<<0);
        delay_ms(20);
        if (!(GPIOB->IDR & (1<<0))) {
            GPIOA->ODR ^= (1<<2);
        }
    }
}

int main(void) {
    RCC->APB2ENR |= (1<<0) | (1<<2) | (1<<3);
	
    GPIOB->CRL &= ~(0xF << 0);
    GPIOB->CRL |=  (0x8 << 0);
    GPIOB->ODR |=  (1<<0);
	
    GPIOA->CRL &= ~(0xF << 8);
    GPIOA->CRL |=  (0x1 << 8);
    GPIOA->ODR |=  (1<<2);
	
    GPIOA->CRL &= ~(0xF << 20);
    GPIOA->CRL |=  (0x1 << 20);
	
    AFIO->EXTICR[0] &= ~(0xF << 0);
    AFIO->EXTICR[0] |=  (0x1 << 0);
	
    EXTI->IMR  |= (1<<0);
    EXTI->FTSR |= (1<<0);
    NVIC_EnableIRQ(EXTI0_IRQn);
	
    TIM2_Init();
	
    while(1) {
        GPIOA->ODR ^= (1<<5);
        delay_ms(1000);
    }
}
