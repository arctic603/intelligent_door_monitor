#ifndef __DHT11_H__
#define __DHT11_H__
#include <stm32f4xx.h>
#include "delay.h"

void DHT11_INIT(uint8_t op);
void DHT11_OUTPUT_MODE(void);
int16_t DHT11_REC_DATA(uint8_t *);
#endif
