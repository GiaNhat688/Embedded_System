#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

typedef struct {
    uint16_t freq;
    uint8_t duty;
} PWM_Params_t;

void GPIO_Config(void);
void vTask_LED(void *pvParameters);
void vTask_Generator(void *pvParameters);

QueueHandle_t xQueue;

void GPIO_Config(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRL &= ~(0xF << (5 * 4));
    GPIOA->CRL |= (0x1 << (5 * 4));
}

void vTask_LED(void *pvParameters) {
    PWM_Params_t params;
    for (;;) {
        if (xQueueReceive(xQueue, &params, portMAX_DELAY) == pdTRUE) {
            float period_ms = 1000.0f / params.freq;
            float on_time = period_ms * params.duty / 100.0f;
            float off_time = period_ms - on_time;
            uint32_t cycles = params.freq;
            for (uint32_t i = 0; i < cycles; i++) {
                GPIOA->BSRR = (1 << 5);
                vTaskDelay(pdMS_TO_TICKS(on_time));
                GPIOA->BRR = (1 << 5);
                vTaskDelay(pdMS_TO_TICKS(off_time));
            }
        }
    }
}

void vTask_Generator(void *pvParameters) {
    PWM_Params_t params;
    uint16_t freq_list[] = {1, 2, 5, 8, 10};
    uint8_t duty_list[] = {10, 30, 50, 70, 90};
    uint8_t idx = 0;
    for (;;) {
        params.freq = freq_list[idx % 5];
        params.duty = duty_list[idx % 5];
        idx++;
        xQueueSend(xQueue, &params, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main(void) {
    GPIO_Config();
    xQueue = xQueueCreate(5, sizeof(PWM_Params_t));
    if (xQueue == NULL)
        while(1);
    xTaskCreate(vTask_LED, "LED", 128, NULL, 1, NULL);
    xTaskCreate(vTask_Generator, "Gen", 128, NULL, 1, NULL);
    vTaskStartScheduler();
    while (1);
}
