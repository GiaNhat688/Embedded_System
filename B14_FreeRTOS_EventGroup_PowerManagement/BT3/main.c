#include "stm32f10x.h"
#include <string.h>

void UART_Init(void) {
    RCC->APB2ENR |= (1 << 2) | (1 << 14);
    GPIOA->CRH &= ~(0xF << 4);
    GPIOA->CRH |= (0xB << 4);
    GPIOA->CRH &= ~(0xF << 8);
    GPIOA->CRH |= (0x4 << 8);
    USART1->BRR = 0x1D4C;
    USART1->CR1 = (1 << 13) | (1 << 3) | (1 << 2);
}

void UART_SendString(const char *s) {
    while (*s) {
        while (!(USART1->SR & (1 << 7)));
        USART1->DR = *s++;
    }
    while (!(USART1->SR & (1 << 6)));
}

void RTC_Init_Wakeup(uint32_t seconds) {
    RCC->APB1ENR |= (1 << 28) | (1 << 27);
    PWR->CR |= (1 << 8);
    RCC->BDCR |= (1 << 0);
    while (!(RCC->BDCR & (1 << 1)));
    RCC->BDCR |= (1 << 8) | (1 << 15);
    while (!(RTC->CRL & (1 << 5)));
    RTC->CRL |= (1 << 4);
    RTC->PRLH = 0;
    RTC->PRLL = 32767;
    uint32_t current_time = ((uint32_t)RTC->CNTH << 16) | RTC->CNTL;
    uint32_t alarm_time = current_time + seconds;
    RTC->ALRH = (alarm_time >> 16) & 0xFFFF;
    RTC->ALRL = alarm_time & 0xFFFF;
    RTC->CRH |= (1 << 1);
    RTC->CRL &= ~(1 << 4);
    while (!(RTC->CRL & (1 << 5)));
    PWR->CR &= ~(1 << 8);
}

void Enter_Standby_Mode(void) {
    PWR->CR |= (1 << 2);
    PWR->CR |= (1 << 1);
    SCB->SCR |= (1 << 2);
    __WFI();
}

int main(void) {
    RCC->APB1ENR |= (1 << 28);
    if (PWR->CSR & (1 << 1)) {
        PWR->CR |= (1 << 3);
        UART_Init();
        UART_SendString("Woke up from Standby!\r\n");
    } else {
        UART_Init();
        UART_SendString("First boot! Configuring RTC...\r\n");
    }
    RTC_Init_Wakeup(5);
    UART_SendString("Going to Standby...\r\n");
    Enter_Standby_Mode();
    while (1);
}

