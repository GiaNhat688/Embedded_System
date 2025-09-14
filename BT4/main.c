#include "stm32f10x.h"

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;
        GPIOA->ODR ^= (1 << 5);
    }
}

void TIM2_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  
    TIM2->PSC = 7200 - 1;
    TIM2->ARR = 5000 - 1;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1  |= TIM_CR1_CEN;
  
    NVIC_EnableIRQ(TIM2_IRQn);
}

int main(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  
    GPIOA->CRL &= ~(0xF << 20);
    GPIOA->CRL |=  (0x1 << 20);
  
    TIM2_Init();
    while (1) {
    }
}
