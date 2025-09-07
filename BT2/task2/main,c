#include "stm32f10x.h"

void GPIO_Config(void) {
    RCC->APB2ENR |= (1 << 2) | (1 << 3);
    GPIOA->CRL &= ~(0xF << 8);
    GPIOA->CRL |=  (0x2 << 8);
    GPIOB->CRL &= ~(0xF << 0);
    GPIOB->CRL |=  (0x8 << 0);
    GPIOB->ODR |=  (1 << 0);
}

void delay_ms(unsigned int t) {
    for (unsigned int i = 0; i < t * 8000; i++) {
        __NOP();
    }
}

int main(void) {
    GPIO_Config();

    int led_state = 0;
    int prev_button = 1;

    while (1) {
        int button = (GPIOB->IDR >> 0) & 0x1;

        if (prev_button == 1 && button == 0) {
            led_state ^= 1;
            if (led_state)
                GPIOA->ODR |=  (1 << 2);
            else
                GPIOA->ODR &= ~(1 << 2);

            delay_ms(200);
        }

        prev_button = button;
    }
}

