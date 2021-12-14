#ifndef __TIM_INIT_H__
#define __TIM_INIT_H__

#include <stm32f4xx.h>

void TIM1_Init(void);
void TIM2_Init(void);
void TIM3_Init(void);
void TIM8_Init(void);
void TIM14_Init(uint16_t t);
void breath_led_init(void);
#endif
