#include "WWDG.h"

static NVIC_InitTypeDef NVIC_InitStruct;

void WWDG_INIT(void)
{
	//使能串口看门狗的硬件时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	//分频值的设置,看门狗的计数器时钟=(42MHZ/4096)/8 ≈ 1281HZ
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	
	//窗口值
	WWDG_SetWindowValue(80);
	
	//配置计数值,同时使能看门狗
	WWDG_Enable(127);			//7位的递减计数器
	
	//配置中断
	NVIC_InitStruct.NVIC_IRQChannel = WWDG_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//清空提前唤醒中断标志位
	WWDG_ClearFlag();
	
	//使能提前唤醒中断
	WWDG_EnableIT();
}

