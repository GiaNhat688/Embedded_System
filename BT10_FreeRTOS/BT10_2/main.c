#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

typedef struct {
    uint16_t pin;
    TickType_t delay;
} LED_Param_t;

void GPIO_Init(void){
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRL &= ~((0xF<<4)|(0xF<<8)|(0xF<<12));
    GPIOA->CRL |=  (0x3<<4)|(0x3<<8)|(0x3<<12);
    GPIOA->ODR &= ~((1<<1)|(1<<2)|(1<<3));
}

void LED_Task(void *pvParameters){
    LED_Param_t *param = (LED_Param_t*) pvParameters;
    for(;;){
        GPIOA->ODR ^= param->pin;
        vTaskDelay(param->delay);
    }
}

int main(void){
    RCC->CR |= RCC_CR_HSION;
    while(!(RCC->CR & RCC_CR_HSIRDY));
    RCC->CFGR = 0;

    GPIO_Init();

    static LED_Param_t led1 = { .pin = (1<<1), .delay = pdMS_TO_TICKS(167) };
    static LED_Param_t led2 = { .pin = (1<<2), .delay = pdMS_TO_TICKS(167) };
    static LED_Param_t led3 = { .pin = (1<<3), .delay = pdMS_TO_TICKS(167) };

    xTaskCreate(LED_Task,"LED_Blink_3Hz",128,&led1,tskIDLE_PRIORITY+1,NULL);
    xTaskCreate(LED_Task,"LED_Blink_10Hz",128,&led2,tskIDLE_PRIORITY+1,NULL);
    xTaskCreate(LED_Task,"LED_Blink_25Hz",128,&led3,tskIDLE_PRIORITY+1,NULL);

    vTaskStartScheduler();

    while(1);
}
