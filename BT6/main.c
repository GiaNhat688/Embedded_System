
#include "stm32f10x.h"
#include <stdio.h>

#define DS1307_ADDR 0xD0

void USART1_Init(void){
    RCC->APB2ENR |= (1<<2) | (1<<14);
	
    GPIOA->CRH &= ~(0xF << 4);
    GPIOA->CRH |=  (0x0B << 4);
	
    GPIOA->CRH &= ~(0xF << 8);
    GPIOA->CRH |=  (0x04 << 8);
	
    USART1->BRR = 0x1D4C;
    USART1->CR1 = (1<<13) | (1<<3) | (1<<2);
}

void USART1_SendChar(char c){
    while (!(USART1->SR & (1<<7)));
    USART1->DR = c;
}

void USART1_SendString(char *s){
    while (*s) USART1_SendChar(*s++);
}

void I2C1_Init(void){
    RCC->APB2ENR |= (1<<3);
	
    RCC->APB1ENR |= (1<<21);
	
    GPIOB->CRL &= ~((0xF << 24) | (0xF << 28));
    GPIOB->CRL |=  ((0xF << 24) | (0xF << 28));
	
    I2C1->CR1 = (1<<15);
    I2C1->CR1 = 0;
    I2C1->CR2 = 36;
    I2C1->CCR = 180;
    I2C1->TRISE = 37;
    I2C1->CR1 |= (1<<0);
}

void I2C1_Start(void){
    I2C1->CR1 |= (1<<8);
    while (!(I2C1->SR1 & (1<<0)));
}

void I2C1_Stop(void){
    I2C1->CR1 |= (1<<9);
}

void I2C1_WriteAddr(uint8_t addr){
    I2C1->DR = addr;
    while (!(I2C1->SR1 & (1<<1)));
    (void)I2C1->SR2;
}

void I2C1_WriteData(uint8_t data){
    while (!(I2C1->SR1 & (1<<7)));
    I2C1->DR = data;
    while (!(I2C1->SR1 & (1<<2)));
}

uint8_t I2C1_ReadData_NACK(void){
    uint8_t data;
	
    I2C1->CR1 &= ~(1<<10);
    while (!(I2C1->SR1 & (1<<6)));
    data = I2C1->DR;
    return data;
}

uint8_t I2C1_ReadReg(uint8_t devAddr, uint8_t regAddr){
    uint8_t data;
	
    I2C1_Start();
    I2C1_WriteAddr(devAddr);
    I2C1_WriteData(regAddr);
	
    I2C1_Start();
    I2C1_WriteAddr(devAddr | 0x01);
	
    data = I2C1_ReadData_NACK();
    I2C1_Stop();
	
    return data;
}

void I2C1_WriteReg(uint8_t devAddr, uint8_t regAddr, uint8_t value){
    I2C1_Start();
    I2C1_WriteAddr(devAddr);
    I2C1_WriteData(regAddr);
    I2C1_WriteData(value);
    I2C1_Stop();
}

uint8_t BCD2DEC(uint8_t val){ 
		return (val>>4)*10 + (val & 0x0F);
}

uint8_t DEC2BCD(uint8_t val){ 
		return ((val/10)<<4) | (val%10); 
}

void DS1307_SetTime(uint8_t hour, uint8_t min, uint8_t sec){
    I2C1_WriteReg(DS1307_ADDR, 0x00, DEC2BCD(sec));
    I2C1_WriteReg(DS1307_ADDR, 0x01, DEC2BCD(min));
    I2C1_WriteReg(DS1307_ADDR, 0x02, DEC2BCD(hour));
}

void DS1307_GetTime(uint8_t *hour, uint8_t *min, uint8_t *sec){
    *sec  = BCD2DEC(I2C1_ReadReg(DS1307_ADDR, 0x00));
    *min  = BCD2DEC(I2C1_ReadReg(DS1307_ADDR, 0x01));
    *hour = BCD2DEC(I2C1_ReadReg(DS1307_ADDR, 0x02));
}

int main(void){
    I2C1_Init();
    USART1_Init();
    DS1307_SetTime(1, 24, 0);
    uint8_t h,m,s;
    char buffer[32];
    while(1){
        DS1307_GetTime(&h, &m, &s);
        sprintf(buffer,"Time: %02d:%02d:%02d AM\r\n",h,m,s);
        USART1_SendString(buffer);
        for(volatile int i=0;i<800000;i++);
    }
}
