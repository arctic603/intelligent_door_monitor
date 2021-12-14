#include "init_led.h"

static GPIO_InitTypeDef GPIO_InitStruct;

void Init_LED(void)
{
	
			//打开端口F的硬件时钟，就是供电
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
			
			//打开端口E的硬件时钟，就是供电
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
			
			GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9; 	//9号引脚
			GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;//复用功能模式
			GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
			GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;//高速，速度越高，响应越快，但是功耗会更高
			GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//不使能上下拉电阻
			GPIO_Init(GPIOF,&GPIO_InitStruct);
			
			//将PF9引脚连接到TIM14
			GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14);
			
			GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10; 	//10号引脚
			GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;//输出模式
			GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
			GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;//高速，速度越高，响应越快，但是功耗会更高
			GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//不使能上下拉电阻
			GPIO_Init(GPIOF,&GPIO_InitStruct);	
			
			GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_14; 	//9 10号引脚
			GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;//输出模式
			GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
			GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;//高速，速度越高，响应越快，但是功耗会更高
			GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;//不使能上下拉电阻	
			
			GPIO_Init(GPIOE,&GPIO_InitStruct);

			GPIO_WriteBit(GPIOF,GPIO_Pin_9,Bit_SET);
			GPIO_WriteBit(GPIOF,GPIO_Pin_10,Bit_SET);
			GPIO_WriteBit(GPIOE,GPIO_Pin_13,Bit_SET);
			GPIO_WriteBit(GPIOE,GPIO_Pin_14,Bit_SET);
	
}
