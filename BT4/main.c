#include "stm32f10x.h"

void TIM2_Init(void) {
    RCC->APB1ENR |= (1 << 0);
    
    TIM2->PSC = 7200 - 1;
    TIM2->ARR = 10 - 1;
    TIM2->CR1  |= (1 << 0);
}

void delay_ms(uint32_t t) {
    for (uint32_t i = 0; i < t; i++) {
        TIM2->CNT = 0;
        while (!(TIM2->SR & 1));
        TIM2->SR &= ~1;
    }
}

int main(void) {
    RCC->APB2ENR |= (1 << 2);
    
    GPIOA->CRL &= ~(0xF << 20);
    GPIOA->CRL |=  (0x2 << 20);
    
    TIM2_Init();
    
    while (1) {
        GPIOA->ODR ^= (1 << 5);
        delay_ms(500);
    }
}
