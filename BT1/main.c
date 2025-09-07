#include "stm32f10x.h"  

void GPIO_Config(){
		RCC->APB2ENR |= (1 << 4);   
    GPIOC->CRH &= ~(0xF << 20); 
    GPIOC->CRH |=  (0x2 << 20);  
    GPIOC->CRH |=  (0x0 << 22);  
}

void delay_ms(volatile unsigned int t) {
    for (unsigned int i = 0; i < t * 8000; i++) {
    }
}

int main(void) {
    GPIO_Config();

    while (1) {
        GPIOC->BSRR = (1 << 13);  
        delay_ms(500);

        GPIOC->BRR  = (1 << 13);  
        delay_ms(500);
    }
}
