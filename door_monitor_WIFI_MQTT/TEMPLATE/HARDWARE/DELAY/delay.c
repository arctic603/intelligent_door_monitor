#include "delay.h"

void delay_xus(uint32_t n)
{

	SysTick->CTRL = 0; 		// Disable SysTick，关闭系统定时器
	SysTick->LOAD = SystemCoreClock/1000000*n-1; // 就是nus
	SysTick->VAL  = 0; 		// Clear current value as well as count flag，清空当前计数值且清零count flag
	SysTick->CTRL = 5; 		// Enable SysTick timer with processor clock，启动系统定时器，其时钟源为处理器时钟168MHz
	while ((SysTick->CTRL & 0x10000)==0);// Wait until count flag is set
	SysTick->CTRL = 0; 		// Disable SysTick，关闭系统定时器
}


void delay_xms(uint32_t n)
{

	while(n--)
	{
		SysTick->CTRL = 0; 		// Disable SysTick，关闭系统定时器
		SysTick->LOAD = SystemCoreClock/1000-1; // Count from 167999 to 0 (168000 cycles)
		SysTick->VAL  = 0; 		// Clear current value as well as count flag，清空当前计数值且清零count flag
		SysTick->CTRL = 5; 		// Enable SysTick timer with processor clock，启动系统定时器，其时钟源为处理器时钟168MHz
		while ((SysTick->CTRL & 0x10000)==0);// Wait until count flag is set
	}
	
	SysTick->CTRL = 0; 		// Disable SysTick，关闭系统定时器
}

