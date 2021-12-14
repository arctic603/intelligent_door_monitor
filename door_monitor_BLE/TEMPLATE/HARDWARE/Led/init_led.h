#ifndef __INIT_LED_H_
#define __INIT_LED_H_

#include <stm32f4xx.h>
#include "sys.h"
//IDR是输入标志寄存器 偏移寄存器结构体地址为0x14,ODR是输出标志寄存器,0x10

extern void Init_LED(void);
int32_t delay_ms(uint32_t nms);
void display_led(void);

#endif 
