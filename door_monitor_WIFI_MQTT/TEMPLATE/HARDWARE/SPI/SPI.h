#ifndef __SPI_H__
#define __SPI_H__

#include <stm32f4xx.h>
#include "delay.h"
#include "sys.h"

/*W25Q128相关引脚宏定义*/
#define W25Q128_CS PBout(14)
#define W25Q128_SCLK PBout(3)
#define W25Q128_MOSI PBout(5)
#define W25Q128_MISO PBin(4)


void W25Q128_INIT(void);
uint8_t SPI1_SendByte(uint8_t byte);
void W25Q128_read_id(uint8_t *m_id,uint8_t *d_id);
void W25Q128_read_data(uint32_t addr,uint8_t *buf,uint32_t len);


void W25Q128_write_enable(void);
void W25Q128_write_disable(void);
uint8_t W25Q128_read_status_1(void);
void W25Q128_erase_sector(uint32_t addr);
void W25Q128_write_page(uint32_t addr,uint8_t *pbuf,uint32_t len);

/*模拟SPI*/
void W25Q128_INIT_MODE(uint8_t ch);
uint8_t SPI1_SendByte_MODE0(uint8_t byte);
uint8_t SPI1_SendByte_MODE3(uint8_t byte);

static SPI_InitTypeDef SPI_InitStructure;
static GPIO_InitTypeDef GPIO_InitStructure;

#endif
