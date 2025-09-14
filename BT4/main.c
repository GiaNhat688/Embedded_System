#include "stm32f10x.h"

void TIM2_IRQHandler(void) {
    if (TIM2->SR & 0x1) {
        TIM2->SR &= ~0x1;
        GPIOA->ODR ^= (1 << 5);
    }
}

void TIM2_Init(void) {
    RCC->APB1ENR |= (1 << 0);
    TIM2->PSC = 7200 - 1;
    TIM2->ARR = 5000 - 1;
    TIM2->DIER |= (1 << 0);
    TIM2->CR1  |= (1 << 0);
	
	NVIC_EnableIRQ(TIM2_IRQn);
}

int main(void) {
    RCC->APB2ENR |= (1 << 2);
	
    GPIOA->CRL &= ~(0xF << 20);
    GPIOA->CRL |=  (0x2 << 20);
	
    TIM2_Init();
	
    while (1) {}
}
