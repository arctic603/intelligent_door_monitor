#include "IWDG.h"

void IWDG_INIT(void)
{
	//解除独立看门狗寄存器的写保护
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	//设置分频值256,独立看门狗的硬件定时器时钟频率=32KHZ/256=125HZ
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	
	//设置计数值就是它的超时时间=1秒
	IWDG_SetReload(125-1);
	
	//刷新独立看门狗的计数值
	IWDG_ReloadCounter();
	
	//使能独立看门狗工作
	IWDG_Enable();
	
}

void IWDG_RST_DETECT(void)
{
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET)
	{
		printf("IWDG RESET CPU!\r\n");
	}else
	{
		printf("NORMAL RESET CPU!\r\n");
	}
	
	RCC_ClearFlag();
}
