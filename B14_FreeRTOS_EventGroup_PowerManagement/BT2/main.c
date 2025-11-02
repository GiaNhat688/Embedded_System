#include "stm32f10x.h"

volatile uint32_t g_delay_counter = 0;

void GPIO_Init(void){
    RCC->APB2ENR |= (1<<2);
	
    GPIOA->CRL &= ~(0xF<<0);;
    GPIOA->CRL |= (0x3<<0); 
}

void SysTick_Init(void){
    SysTick->LOAD = 8000 - 1;
		SysTick->VAL = 0;
		SysTick->CTRL |= (1<<0|1<<1|1<<2);
}

void SysTick_Handler(void)
{
    if(g_delay_counter > 0){
			g_delay_counter--;
		}	
}

void Delay_ms_sleep(uint32_t ms){
    g_delay_counter = ms;
    while(g_delay_counter > 0){
        __WFI(); 
    }
}

int main(void){
    GPIO_Init();
    SysTick_Init();
	
    SCB->SCR &= ~(1<<2);

    while(1){
        GPIOA->BSRR |= (1<<0); 
        Delay_ms_sleep(1000);
			
        GPIOA->BRR |= (1<<0); 
        Delay_ms_sleep(1000);
    }
}
