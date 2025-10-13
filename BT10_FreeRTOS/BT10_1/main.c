#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

void GPIO_Init(void){
    RCC->APB2ENR |= 1<<2;
    GPIOA->CRL &= ~((0xF<<4)|(0xF<<8)|(0xF<<12));
    GPIOA->CRL |=  (0x3<<4)|(0x3<<8)|(0x3<<12); // Output 50MHz
    GPIOA->ODR &= ~((1<<1)|(1<<2)|(1<<3));
}

//pa1
void Task01(void *pvParameters){
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(167); // 3Hz

    for(;;){
        GPIOA->ODR ^= (1<<1);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

//pa2
void Task02(void *pvParameters){
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(50); // 10Hz

    for(;;){
        GPIOA->ODR ^= (1<<2);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

//pa3
void Task03(void *pvParameters){
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(20); // 25Hz

    for(;;){
        GPIOA->ODR ^= (1<<3);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

int main(void){
    RCC->CR |= 1<<0;
    while(!(RCC->CR & (1<<1)));
    RCC->CFGR = 0;

    GPIO_Init();

    xTaskCreate(Task01,"Led_Blink_3Hz",128,NULL,tskIDLE_PRIORITY+1,NULL);
    xTaskCreate(Task02,"Led_Blink_10Hz",128,NULL,tskIDLE_PRIORITY+1,NULL);
    xTaskCreate(Task03,"Led_Blink_25Hz",128,NULL,tskIDLE_PRIORITY+1,NULL);

    vTaskStartScheduler();

    while(1);
}
