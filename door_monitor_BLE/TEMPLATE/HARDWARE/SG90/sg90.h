#ifndef __SG90_H__
#define __SG90_H__

#include <stm32f4xx.h>
#include "sys.h"

static uint32_t g_pwm_cnt=0;

void sg_init(void);
void sg_angle(uint32_t angle);

#endif

