#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

void GPIO_Init(void) {
    RCC->APB2ENR |= (1 << 2);
    GPIOA->CRL &= ~(0xF << 0);
    GPIOA->CRL |=  (0x3 << 0);
}

void UART_Init(void) {
    RCC->APB2ENR |= (1 << 2) | (1 << 14);
    GPIOA->CRH &= ~(0xF << 4);
    GPIOA->CRH |=  (0xB << 4);
    GPIOA->CRH &= ~(0xF << 8);
    GPIOA->CRH |=  (0x4 << 8);
    USART1->BRR = 0x1D4C;
    USART1->CR1 = (1 << 13) | (1 << 3) | (1 << 2);
}

void UART_SendString(const char *s) {
    taskENTER_CRITICAL();
    while (*s) {
        while (!(USART1->SR & (1 << 7)));
        USART1->DR = *s++;
    }
    while (!(USART1->SR & (1 << 6)));
    taskEXIT_CRITICAL();
}

void vBlinkTask(void *pvParameters) {
    for (;;) {
        GPIOA->ODR ^= (1 << 0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void vUARTTask(void *pvParameters) {
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(2000));
        UART_SendString("Tasks running... CPU was sleeping.\r\n");
    }
}

void vApplicationIdleHook(void) {
    SCB->SCR &= ~(1 << 2);
    __WFI();
}

int main(void) {
    GPIO_Init();
    UART_Init();
	
    xTaskCreate(vBlinkTask, "Blink", 128, NULL, 1, NULL);
    xTaskCreate(vUARTTask, "UART", 128, NULL, 1, NULL);
    vTaskStartScheduler();
    while (1);
}

