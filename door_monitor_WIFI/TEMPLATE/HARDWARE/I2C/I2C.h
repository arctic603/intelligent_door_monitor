#ifndef __I2C_H__
#define __I2C_H__
#include <stdio.h>
#include <stm32f4xx.h>
#include "sys.h"
#include "delay.h"

#define SCL_W PBout(8)
#define SDA_W PBout(9)
#define SDA_R PBin(9)

static GPIO_InitTypeDef 	 GPIO_InitStructure;


void I2C_INIT(void);
void SDA_Pin_Mode(GPIOMode_TypeDef Pin_Mode);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Send_Byte(uint8_t byte);
uint8_t I2C_Recv_Byte(void);
uint8_t I2C_Wait_ACK(void);
int32_t AT24C02_Write(uint8_t addr,uint8_t *Buf,uint8_t len);
void I2C_ACK(uint8_t ack);
int32_t AT24C02_Read(uint8_t addr,uint8_t *buf,uint8_t len);


#endif
