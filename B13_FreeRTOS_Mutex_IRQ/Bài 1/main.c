#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdbool.h>

SemaphoreHandle_t xInterruptSemaphore;
volatile bool isWarningOn = false;
#define DEBOUNCE_TIME_MS 200

void Hardware_Init(void){
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN| RCC_APB2ENR_AFIOEN;

    GPIOA->CRL &= ~(0xF << 20);
    GPIOA->CRL |= (0x3 << 20);

    GPIOA->CRL &= ~(0xF << 4);
    GPIOA->CRL |= (0x3 << 4);

    GPIOA->CRL &= ~(0xF << 0);
    GPIOA->CRL |= (0x8 << 0);
    GPIOA->ODR |= (1 << 0);

    AFIO->EXTICR[0] &= ~AFIO_EXTICR1_EXTI0;
    EXTI->IMR |= EXTI_IMR_MR0;
    EXTI->FTSR |= EXTI_FTSR_TR0;

    NVIC_SetPriority(EXTI0_IRQn, 5);
    NVIC_EnableIRQ(EXTI0_IRQn);
}

void vBlinkTask(void *pvParameters){
    for(;;){
        GPIOA->ODR ^= (1 << 5);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void vWarningTask(void *pvParameters){
    for (;;){
        if (xSemaphoreTake(xInterruptSemaphore, portMAX_DELAY) == pdTRUE){
            isWarningOn = !isWarningOn;

            if (isWarningOn){
                GPIOA->BSRR = (1 << 1);
            } else{
                GPIOA->BRR = (1 << 1);
            }
        }
    }
}

void EXTI0_IRQHandler(void) {
    static TickType_t lastInterruptTime = 0;
    TickType_t currentInterruptTime = xTaskGetTickCountFromISR();

    if(EXTI->PR & EXTI_PR_PR0){
        EXTI->PR |= EXTI_PR_PR0;
    }

    if((currentInterruptTime - lastInterruptTime) > pdMS_TO_TICKS(DEBOUNCE_TIME_MS)){
        lastInterruptTime = currentInterruptTime;

        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
			
        xSemaphoreGiveFromISR(xInterruptSemaphore, &xHigherPriorityTaskWoken);
			
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

int main(void){
    Hardware_Init();
    xInterruptSemaphore = xSemaphoreCreateBinary();

    if(xInterruptSemaphore != NULL){
        xTaskCreate(vBlinkTask, "Blink", 128, NULL, 1, NULL);
        xTaskCreate(vWarningTask, "Warning", 128, NULL, 2, NULL);
        vTaskStartScheduler();
    }

    for (;;);
}
