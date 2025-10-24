#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdbool.h>

SemaphoreHandle_t xInterruptSemaphore;
volatile bool isOn = false;
#define DEBOUNCE_TIME_MS 200

void Hardware_Init(void) {
    RCC->APB2ENR |= (1 << 0) | (1 << 2);

    GPIOA->CRL &= ~(0xF << 0);
    GPIOA->CRL |= (0x8 << 0);
    GPIOA->ODR |= (1 << 0);

    GPIOA->CRL &= ~(0xF << 4);
    GPIOA->CRL |= (0x3 << 4);

    GPIOA->CRL &= ~(0xF << 20);
    GPIOA->CRL |= (0x3 << 20);

    AFIO->EXTICR[0] &= ~(0xF << 0);
    EXTI->IMR |= (1 << 0);
    EXTI->FTSR |= (1 << 0);

    NVIC_SetPriority(EXTI0_IRQn, 5);
    NVIC_EnableIRQ(EXTI0_IRQn);
}

void vBlinkTask(void *pvParameters) {
    for (;;) {
        GPIOA->ODR ^= (1 << 5);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vInterruptTask(void *pvParameters) {
    for (;;) {
        if (xSemaphoreTake(xInterruptSemaphore, portMAX_DELAY) == pdTRUE) {
            isOn = !isOn;
            if (isOn) {
                GPIOA->BSRR |= (1 << 1);
            } else {
                GPIOA->BRR = (1 << 1);
            }
        }
    }
}

void EXTI0_IRQHandler(void) {
    static TickType_t lastTime = 0;
    TickType_t currentTime = xTaskGetTickCountFromISR();

    if (EXTI->PR & (1 << 0)) {
        EXTI->PR |= (1 << 0);
    }

    if ((currentTime - lastTime) > pdMS_TO_TICKS(DEBOUNCE_TIME_MS)) {
        lastTime = currentTime;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(xInterruptSemaphore, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

int main(void) {
    Hardware_Init();
    xInterruptSemaphore = xSemaphoreCreateBinary();

    if (xInterruptSemaphore != NULL) {
        xTaskCreate(vBlinkTask, "Blink", 128, NULL, 1, NULL);
        xTaskCreate(vInterruptTask, "Interrupt", 128, NULL, 2, NULL);
        vTaskStartScheduler();
    }

    for (;;);
}
