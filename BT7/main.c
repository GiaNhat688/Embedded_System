
#include "stm32f10x.h"
#include <stdio.h>

#define CS_LOW()   (GPIOA->BSRR = (1<<20))   // PA4 = 0
#define CS_HIGH()  (GPIOA->BSRR = (1<<4))    // PA4 = 1

#define ADXL345_DEVID  0x00

void delay_ms(volatile uint32_t ms){
    for(uint32_t i=0;i<ms*8000;i++);
}

void USART1_Init(void){
    RCC->APB2ENR |= (1<<2) | (1<<14);   // GPIOA + USART1

    // PA9 (TX) AF push-pull
    GPIOA->CRH &= ~(0xF<<4);
    GPIOA->CRH |=  (0x0B<<4);
    // PA10 (RX) input floating
    GPIOA->CRH &= ~(0xF<<8);
    GPIOA->CRH |=  (0x04<<8);

    USART1->BRR = 0x1D4C;               // 9600 baud @72MHz
    USART1->CR1 = (1<<13)|(1<<3)|(1<<2); // UE, TE, RE
}

void USART1_SendChar(char c){
    while(!(USART1->SR & (1<<7)));
    USART1->DR = c;
}

void USART1_SendString(char *s){
    while(*s) USART1_SendChar(*s++);
}

void SPI1_Init(void){
    RCC->APB2ENR |= (1<<2) | (1<<12);   // GPIOA + SPI1

    // PA4 CS output push-pull
    GPIOA->CRL &= ~(0xF<<16);
    GPIOA->CRL |=  (0x3<<16);

    // PA5 SCK AF push-pull
    GPIOA->CRL &= ~(0xF<<20);
    GPIOA->CRL |=  (0xB<<20);

    // PA6 MISO input floating
    GPIOA->CRL &= ~(0xF<<24);
    GPIOA->CRL |=  (0x04<<24);

    // PA7 MOSI AF push-pull
    GPIOA->CRL &= ~(0xF<<28);
    GPIOA->CRL |=  (0x0B<<28);

    CS_HIGH();

    // SPI1 config: Master, SSM=1, SSI=1, Baud fPCLK/16
    SPI1->CR1 = (1<<6)|(1<<2)|(1<<9)|(1<<8)|(0x3<<3);
    // SPE=1, MSTR=1, SSM=1, SSI=1
}

uint8_t SPI1_TransmitReceive(uint8_t data){
    while(!(SPI1->SR & (1<<1))); // TXE
    SPI1->DR = data;
    while(!(SPI1->SR & (1<<0))); // RXNE
    return SPI1->DR;
}

uint8_t ADXL345_Read(uint8_t reg){
    uint8_t val;
    CS_LOW();
    SPI1_TransmitReceive(reg | 0x80); // Read bit
    val = SPI1_TransmitReceive(0x00);
    CS_HIGH();
    return val;
}

int main(void){
    USART1_Init();
    SPI1_Init();

    while(1){
        uint8_t id = ADXL345_Read(ADXL345_DEVID);
        char buf[32];
        sprintf(buf,"ADXL345 ID: 0x%02X\r\n",id);
        USART1_SendString(buf);

        delay_ms(1000);
    }
}
