#include "stm32f10x.h"
#include "string.h"

void GPIO_init(void){
    RCC->APB2ENR |= (1<<2);  
    RCC->APB1ENR |= (1<<17);  

    GPIOA->CRL &= ~(0xF << (0*4));
    GPIOA->CRL |=  (0x3 << (0*4));

    GPIOA->CRL &= ~(0xF << (2*4));
    GPIOA->CRL |=  (0xB << (2*4));

    GPIOA->CRL &= ~(0xF << (3*4));
    GPIOA->CRL |=  (0x4 << (3*4));
}

void USART2_Init(uint32_t baud){
    USART2->BRR = 36000000 / baud;
    USART2->CR1 |= (1<<13) | (1<<2) | (1<<3);
}

void USART2_SendChar(char c){
    while(!(USART2->SR & (1<<7)));
    USART2->DR = c;
}

void USART2_SendString(char *s){
    while(*s){
        USART2_SendChar(*s++);
    }
}

char USART2_GetChar(void){
    while(!(USART2->SR & (1<<5)));
    return USART2->DR;
}

int main(void){
    char buffer[4] = {0};
    int idx = 0;

    GPIO_init();
    USART2_Init(9600);

    GPIOA->BRR = (1<<0);

    USART2_SendString("Hello from STM32!\r\n");

    while(1){
        char c = USART2_GetChar();
        buffer[idx++] = c;

        if(idx == 2){
            buffer[2] = '\0';
            if(strcmp(buffer, "ON") == 0){
                GPIOA->BSRR = (1<<0);
                USART2_SendString("LED ON\r\n");
                idx = 0;
                continue;
            }
        }

        if(idx == 3){
            buffer[3] = '\0';
            if(strcmp(buffer, "OFF") == 0){
                GPIOA->BRR = (1<<0);
                USART2_SendString("LED OFF\r\n");
                idx = 0;
                continue;
            }
        }

        if(idx >= 3){
            idx = 0;
        }
    }
}
