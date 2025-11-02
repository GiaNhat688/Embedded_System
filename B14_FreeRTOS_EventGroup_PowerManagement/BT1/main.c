#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

//event bits
//start + done bits
#define task1_start_bit (1<<0)
#define task2_start_bit (1<<1)
#define task3_start_bit (1<<2)
#define all_start_bit (task1_start_bit|task2_start_bit|task3_start_bit)

#define task1_done_bit (1<<4)
#define task2_done_bit (1<<5)
#define task3_done_bit (1<<6)
#define all_done_bit (task1_done_bit|task2_done_bit|task3_done_bit)

//handle
EventGroupHandle_t xMyEventGroup;

//gpio
void GPIO_Init(void){
	//bat clock
	RCC->APB2ENR |= (1<<2);
	
	//pa0,1,2 -> output pp 50Mhz
	GPIOA->CRL &= ~(0xF<<0);
	GPIOA->CRL |= (0x3<<0);
	
	GPIOA->CRL &= ~(0xF<<4);
	GPIOA->CRL |= (0x3<<4);
	
	GPIOA->CRL &= ~(0xF<<8);
	GPIOA->CRL |= (0x3<<8);
}

//task dieu phoi
void vCoordinatorTask(void *pvParameters){
	for(;;){
		//kich hoat don
		//gui bit can bat, sau do nhan va thuc thi task1, cho bit duoc set sau khi lam xong task1
		vTaskDelay(pdMS_TO_TICKS(1000));
		
		//task1
		xEventGroupSetBits(xMyEventGroup, task1_start_bit);
		xEventGroupWaitBits(xMyEventGroup, task1_done_bit, pdTRUE, pdTRUE, portMAX_DELAY);
		
		//taks2
		xEventGroupSetBits(xMyEventGroup, task2_start_bit);
		xEventGroupWaitBits(xMyEventGroup, task2_done_bit, pdTRUE, pdTRUE, portMAX_DELAY);
		
		//task3
		xEventGroupSetBits(xMyEventGroup, task3_start_bit);
		xEventGroupWaitBits(xMyEventGroup, task3_done_bit, pdTRUE, pdTRUE, portMAX_DELAY);
		
		//dong thoi
		vTaskDelay(pdMS_TO_TICKS(2000));
		
		xEventGroupSetBits(xMyEventGroup, all_start_bit);
		xEventGroupWaitBits(xMyEventGroup, all_done_bit, pdTRUE, pdTRUE, portMAX_DELAY);
	}
}

void vWorker1(void *pvParameters){
	for(;;){
		//cho tc
		xEventGroupWaitBits(xMyEventGroup, task1_start_bit, pdTRUE, pdTRUE, portMAX_DELAY);
		
		//lam viec
		GPIOA->BSRR |= (1<<0);
		vTaskDelay(pdMS_TO_TICKS(300));
		GPIOA->BRR |= (1<<0);
		vTaskDelay(pdMS_TO_TICKS(300));
		
		xEventGroupSetBits(xMyEventGroup, task1_done_bit);
	}
}

void vWorker2 (void *pvParameters){
	for(;;){
		//cho tc
		xEventGroupWaitBits(xMyEventGroup, task2_start_bit, pdTRUE, pdTRUE, portMAX_DELAY);
		
		GPIOA->BSRR |= (1<<1);
		vTaskDelay(pdMS_TO_TICKS(300));
		GPIOA->BRR |= (1<<1);
		vTaskDelay(pdMS_TO_TICKS(300));
		
		xEventGroupSetBits(xMyEventGroup, task2_done_bit);
	}
}

void vWorker3(void *pvParameters){
	for(;;){
		xEventGroupWaitBits(xMyEventGroup, task3_start_bit, pdTRUE, pdTRUE, portMAX_DELAY);
		
		GPIOA->BSRR |= (1<<2);
		vTaskDelay(pdMS_TO_TICKS(300));
		GPIOA->BRR |= (1<<2);
		vTaskDelay(pdMS_TO_TICKS(300));
		
		xEventGroupSetBits(xMyEventGroup, task3_done_bit);
	}
}

int main(void){
	GPIO_Init();
	
	xMyEventGroup = xEventGroupCreate();
	
	xTaskCreate(vCoordinatorTask, "Coord", 128, NULL, 2, NULL);
	xTaskCreate(vWorker1, "Worker1", 128, NULL, 1, NULL);
	xTaskCreate(vWorker2, "Worker2", 128, NULL, 1, NULL);
	xTaskCreate(vWorker3, "Worker3", 128, NULL, 1, NULL);
	
	vTaskStartScheduler();
	
	while(1);
}
