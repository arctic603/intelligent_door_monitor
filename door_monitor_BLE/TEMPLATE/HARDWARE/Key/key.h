#ifndef __KEY_H__
#define __KEY_H__

#include <stm32f4xx.h>
#include "sys.h"

void display_key_func(void);

extern void key_init(void);
extern void matrix_key_init(void);
extern uint32_t exti_key_sta_get(void);

char get_key_status(void);
char loop_get_key_sta(void);
void key_stabilize(GPIO_TypeDef* GPIOx,u16 GPIO_Pin,char *ch,int line);
#endif
