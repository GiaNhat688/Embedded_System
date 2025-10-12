#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

void GPIO_Init(void){
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRL &= ~((0xF<<4)|(0xF<<8)|(0xF<<12));
    GPIOA->CRL |=  (0x3<<4)|(0x3<<8)|(0x3<<12); // Output 50MHz
    GPIOA->ODR &= ~((1<<1)|(1<<2)|(1<<3));
}

void Task01(void *pvParameters){
    for(;;){
        GPIOA->ODR ^= (1<<1); // Toggle PA1
        vTaskDelay(167 / portTICK_PERIOD_MS); // 3Hz
    }
}

void Task02(void *pvParameters){
    for(;;){
        GPIOA->ODR ^= (1<<2); // Toggle PA2
        vTaskDelay(50 / portTICK_PERIOD_MS); // 10Hz
    }
}

void Task03(void *pvParameters){
    for(;;){
        GPIOA->ODR ^= (1<<3); // Toggle PA3
        vTaskDelay(20 / portTICK_PERIOD_MS); // 25Hz
    }
}

int main(void){
    RCC->CR |= RCC_CR_HSION;
    while(!(RCC->CR & RCC_CR_HSIRDY));
    RCC->CFGR = 0;

    GPIO_Init();

    xTaskCreate(Task01,"Task01",128,NULL,tskIDLE_PRIORITY+1,NULL);
    xTaskCreate(Task02,"Task02",128,NULL,tskIDLE_PRIORITY+1,NULL);
    xTaskCreate(Task03,"Task03",128,NULL,tskIDLE_PRIORITY+1,NULL);

    vTaskStartScheduler();

    while(1);
}

