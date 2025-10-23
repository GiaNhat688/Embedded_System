#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h" 

SemaphoreHandle_t xUsartMutex;
SemaphoreHandle_t xSyncSemaphore;

void USART1_Config(void){
	RCC->APB2ENR |= (1<<2)|(1<<14); 
	
	GPIOA->CRH &= ~(0xF<<4);
	GPIOA->CRH |= (0xB<<4);
	
	GPIOA->CRH &= ~(0xF<<8);
	GPIOA->CRH |= (0x4<<8);
	
	USART1->BRR = 0x0341;
	USART1->CR1 |= (1<<2)|(1<<3)|(1<<13); 
}

void USART1_SendString_Unsafe(const char *str){
	while(*str){
		while(!(USART1->SR & (1<<7)));
		USART1->DR = (*str++);
	}
}

void USART1_SendString_Safe(const char *str){
	if(xSemaphoreTake(xUsartMutex, portMAX_DELAY)==pdTRUE){
		while(*str){
			while(!(USART1->SR & (1<<7)));
			USART1->DR = (*str++);
		}
		xSemaphoreGive(xUsartMutex);
	}
}

void vTask1(void *pvParameters){
	for(;;){
		USART1_SendString_Unsafe("1234\r\n");
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void vTask2(void *pvParameters){
	for(;;){
		USART1_SendString_Unsafe("ABCD\r\n");
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void vTask3(void *pvParameters){
	xSemaphoreTake(xSyncSemaphore, 0);
	
	USART1_SendString_Safe("Hello Gia Nhat!\r\n");
	
	xSemaphoreGive(xSyncSemaphore);
	
	for(;;){
		USART1_SendString_Safe("Hello Gia Nhat!\r\n");
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void vTask4(void *pvParameters){
	xSemaphoreTake(xSyncSemaphore, portMAX_DELAY);
	
	for(;;){
		USART1_SendString_Safe("Good Morning!\r\n");
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

int main(void){
	USART1_Config();
			
	xUsartMutex = xSemaphoreCreateMutex();
	xSyncSemaphore = xSemaphoreCreateBinary();
	
	if (xUsartMutex != NULL && xSyncSemaphore != NULL){
		xTaskCreate(vTask1, "String1", 128, NULL, 1, NULL);
		xTaskCreate(vTask2, "String2", 128, NULL, 1, NULL);
		xTaskCreate(vTask3, "String3", 128, NULL, 1, NULL);
		xTaskCreate(vTask4, "String4", 128, NULL, 1, NULL);
		
		vTaskStartScheduler();
	}
	for(;;);
}
