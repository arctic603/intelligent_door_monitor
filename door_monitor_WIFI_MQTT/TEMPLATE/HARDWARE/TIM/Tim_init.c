#include "Tim_init.h"

static TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
static NVIC_InitTypeDef 	   NVIC_InitStruct;
static TIM_OCInitTypeDef  	   TIM_OCInitStruct;
void TIM3_Init(void)		//500ms定时中断
{
	//使能TIM3的硬件时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	//配置TIM3的定时时间
	TIM_TimeBaseStruct.TIM_Period = 20000/2-1;		//自动重载值
	TIM_TimeBaseStruct.TIM_Prescaler = 4200 - 1;	//预分频值为4200
	TIM_TimeBaseStruct.TIM_ClockDivision = 0; 
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;	//从0开始计数至TIM_Period时触发中断
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStruct);
	
	//配置TIM3的中断触发：时间更新
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	//配置TIM3的优先级
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;				//中断号
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;		//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;				//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;				//打开通道给NVIC管理
	NVIC_Init(&NVIC_InitStruct);
	
	//使能TIM3工作
	TIM_Cmd(TIM3,ENABLE);
}

void TIM1_Init(void)		//100ms定时中断
{
	//使能TIM1的硬件时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	//配置TIM1的定时时间
	TIM_TimeBaseStruct.TIM_Period = 4000-1;		//自动重载值
	TIM_TimeBaseStruct.TIM_Prescaler = 4200-1;	//预分频值为4200
	TIM_TimeBaseStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;	//从0开始计数至TIM_Period时触发中断
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStruct);
	
	//配置TIM3的中断触发：时间更新
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	
	//配置TIM3的优先级
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;				//中断号
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;		//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;				//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;				//打开通道给NVIC管理
	NVIC_Init(&NVIC_InitStruct);
	
	//使能TIM3工作
	TIM_Cmd(TIM1,ENABLE);
}

void TIM2_Init(void)		//200ms定时中断
{
	//使能TIM2的硬件时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//配置TIM3的定时时间
	TIM_TimeBaseStruct.TIM_Period = 1000-1;		//自动重载值
	TIM_TimeBaseStruct.TIM_Prescaler = 8400 - 1;	
	TIM_TimeBaseStruct.TIM_ClockDivision = 0; 
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;	//从0开始计数至TIM_Period时触发中断
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStruct);
	
	//配置TIM3的中断触发：时间更新
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	//配置TIM3的优先级
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;				//中断号
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;		//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;				//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;				//打开通道给NVIC管理
	NVIC_Init(&NVIC_InitStruct);
	
	//使能TIM3工作
	TIM_Cmd(TIM2,ENABLE);

}

void TIM8_Init(void) 
{
	//使能TIM8的硬件时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
	
	//配置TIM8的定时时间
	TIM_TimeBaseStruct.TIM_Period = 40000-1;		//自动重载值
	TIM_TimeBaseStruct.TIM_Prescaler = 16800 - 1;  
	TIM_TimeBaseStruct.TIM_ClockDivision = 0; 
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;	//从0开始计数至TIM_Period时触发中断
	
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStruct);
	
	//配置TIM8的中断触发：时间更新
	TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE);
	
	//配置TIM8的优先级
	NVIC_InitStruct.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;				//中断号
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;		//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;				//响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;				//打开通道给NVIC管理
	NVIC_Init(&NVIC_InitStruct);
	
	//使能TIM4工作
	TIM_Cmd(TIM8,ENABLE);

}

void TIM14_Init(uint16_t t)
{
	//使能TIM14的硬件时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);

	//配置TIM14的定时时间
	TIM_TimeBaseStruct.TIM_Period = 500-1;
	TIM_TimeBaseStruct.TIM_Prescaler = 8400-1;
	TIM_TimeBaseStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStruct);
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputNState_Enable;	//
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;	//有效状态为高电平
	
	//初始化TIM14通道
	TIM_OC1Init(TIM14,&TIM_OCInitStruct);
	
	//使能TIM14工作
	TIM_Cmd(TIM14,ENABLE);
	
}

static TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
static TIM_OCInitTypeDef  TIM_OCInitStructure;
static GPIO_InitTypeDef GPIO_InitStruct;

void breath_led_init(void)
{	
	/* GPIOA, GPIOB and GPIOE Clocks enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);
	
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	TIM_TimeBaseStructure.TIM_Period = 100-1;        //10000
	TIM_TimeBaseStructure.TIM_Prescaler = 8399;   	 //84000000
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //0数到99 down的话就是从99数到0；
	
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Period = 100-1;        //10000
	TIM_TimeBaseStructure.TIM_Prescaler = 16799;   	 //168000000
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //0数到99 down的话就是从99数到0；
	
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 99;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 99;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM14, ENABLE);
	
}

