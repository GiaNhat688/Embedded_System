#include "stm32f10x.h"
#include <stdio.h>

#define DS1307_ADDR 0xD0   // 8-bit Write Address (0x68<<1)

void USART1_Init(void){
    RCC->APB2ENR |= (1<<2);           // GPIOA
    RCC->APB2ENR |= (1<<14);          // USART1
    
    GPIOA->CRH &= ~(0xF << 4);        // PA9
    GPIOA->CRH |=  (0x0B << 4);       // AF-PP
    
    GPIOA->CRH &= ~(0xF << 8);        // PA10
    GPIOA->CRH |=  (0x04 << 8);       // Input floating
    
    USART1->BRR = 0x1D4C;             // Baudrate 9600 @72MHz
    
    USART1->CR1 = 0;
    USART1->CR1 |= (1<<13);           // UE
    USART1->CR1 |= (1<<3);            // TE
    USART1->CR1 |= (1<<2);            // RE
}

void USART1_SendChar(char c){
    while (!(USART1->SR & (1<<7)));
    USART1->DR = c;
}

void USART1_SendString(char *s){
    while (*s){
        USART1_SendChar(*s++);
    }
}

void I2C1_Init(void){
    RCC->APB2ENR |= (1<<3);           // GPIOB
    RCC->APB1ENR |= (1<<21);          // I2C1
    
    GPIOB->CRL &= ~(0xF << 24);       // PB6
    GPIOB->CRL |=  (0xF << 24);       // AF-OD
    
    GPIOB->CRL &= ~(0xF << 28);       // PB7
    GPIOB->CRL |=  (0xF << 28);       // AF-OD
    
    I2C1->CR1 = (1<<15);              // SWRST
    I2C1->CR1 = 0;
    
		//cau hinh toc do i2c 100khz
    I2C1->CR2 = 36;                   // PCLK1 = 36MHz
    I2C1->CCR = 180;                  // 100kHz   //pclk/2*i2c_speed
    I2C1->TRISE = 37;                 // Trise + 1
    
    I2C1->CR1 |= (1<<0);              // PE
}

void I2C1_Start(void){
    I2C1->CR1 |= (1<<8);              // START
    while (!(I2C1->SR1 & (1<<0)));    // Wait SB
}

void I2C1_Stop(void){
    I2C1->CR1 |= (1<<9);              // STOP
}

void I2C1_WriteAddr(uint8_t addr){
    I2C1->DR = addr;
    while (!(I2C1->SR1 & (1<<1)));    // Wait ADDR
    (void)I2C1->SR2;                  // Clear ADDR
}

void I2C1_WriteData(uint8_t data){
    while (!(I2C1->SR1 & (1<<7)));    // TXE
    I2C1->DR = data;
    while (!(I2C1->SR1 & (1<<2)));    // BTF
}

uint8_t I2C1_ReadData_NACK(void){
    while (!(I2C1->SR1 & (1<<6)));    // RXNE
    return I2C1->DR;
}

void I2C1_ReadAddr_1Byte(uint8_t addr){
    I2C1->DR = addr;
    while (!(I2C1->SR1 & (1<<1)));    // ADDR
    I2C1->CR1 &= ~(1<<10);            // ACK=0
    (void)I2C1->SR2;                  // Clear ADDR
}

//START -> (Addr + W) -> RegAddr -> Re-START -> (Addr + R) -> Data (NACK) -> STOP
//gui dia chi slave -> gui dia chi ben trong thanh ghi cua slave -> sang che do doc -> nack -> ket thuc doc
//doc 1 thanh ghi va tra ve du lieu doc duoc i2c slave
uint8_t I2C1_ReadReg(uint8_t devAddr, uint8_t regAddr){
    uint8_t data;
    
    I2C1_Start();
    I2C1_WriteAddr(devAddr);
    I2C1_WriteData(regAddr);
    
    I2C1_Start();
    I2C1_ReadAddr_1Byte(devAddr | 0x01); //doc bit cuoi = 1; ghi thi bit cuoi = 0 (w = add << 1, r = add |= 0x01 -> 0xD1
    
    data = I2C1_ReadData_NACK();
    
    I2C1_Stop();
    
    return data;
}

//START -> (Addr + W) -> RegAddr -> Data -> STOP

//dev-dia chi slave: 0xD0. reg-thanh ghi ben trong ds1307: 0x00: giay, 0x01:phut,...
//value:gia tri can ghi
void I2C1_WriteReg(uint8_t devAddr, uint8_t regAddr, uint8_t val){
    I2C1_Start();
	
	//gui dia chi slave + bit ghi (w=0)
    I2C1_WriteAddr(devAddr);
	//dia chi thanh ghi ben trong muon ghi
    I2C1_WriteData(regAddr);
	//gui gia tri can ghi
    I2C1_WriteData(val);
	
    I2C1_Stop();
}

uint8_t BCD2DEC(uint8_t v){
	//chuyen tu bcd->decimal
	//0x12: v>>4=1 * 10 =10; v&0x0F=2 -> 12
    return (v >> 4) * 10 + (v & 0x0F);
}

uint8_t DEC2BCD(uint8_t v){
    return ((v / 10) << 4) | (v % 10);
}

void DS1307_SetTime(uint8_t h, uint8_t m, uint8_t s){
	//thanh ghi giay cua ds1307 co 7 bit, bit 7 co ch=1 dung, ch=0 chay
    I2C1_WriteReg(DS1307_ADDR, 0x00, DEC2BCD(s) & 0x7F); // clear CH
    I2C1_WriteReg(DS1307_ADDR, 0x01, DEC2BCD(m));
    I2C1_WriteReg(DS1307_ADDR, 0x02, DEC2BCD(h));
}

void DS1307_GetTime(uint8_t *h, uint8_t *m, uint8_t *s){
    *s = BCD2DEC(I2C1_ReadReg(DS1307_ADDR, 0x00));
    *m = BCD2DEC(I2C1_ReadReg(DS1307_ADDR, 0x01));
	
		//thanh ghi gio co bit 6 - che do 12/24h
		//bit 5:am/pm
		//bit 4-0: gia tri gio
    *h = BCD2DEC(I2C1_ReadReg(DS1307_ADDR, 0x02) & 0x3F); // mask 24h
}

int main(void){
    I2C1_Init();
    USART1_Init();
    
    DS1307_SetTime(1, 24, 0);
    
    uint8_t h, m, s;
    char buf[32];
    
    while (1){
        DS1307_GetTime(&h, &m, &s);
        
        sprintf(buf, "Time: %02d:%02d:%02d\r\n", h, m, s);
        USART1_SendString(buf);
        
        for (volatile int i = 0; i < 800000; i++);
    }
}
