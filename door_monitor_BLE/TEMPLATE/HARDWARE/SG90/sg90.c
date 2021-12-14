#include "sg90.h"

static GPIO_InitTypeDef 		GPIO_InitStructure;
static TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
static TIM_OCInitTypeDef  		TIM_OCInitStructure;

void sg_init(void)
{
	//使能端口A时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	/* TIM3 clock enable ，定时器的时钟使能*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/* 配置PA6 为PWM输出模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;					//第6根引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//设置为复用功能模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//推挽模式，增加驱动电流
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//设置IO的速度为100MHz，频率越高性能越好，频率越低，功耗越低
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//不需要上拉电阻
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);

	/* Time base configuration，定时器的基本配置，用于配置定时器的输出脉冲的频率为50Hz */
	TIM_TimeBaseStructure.TIM_Period = 10000/50;						//设置定时的频率为50Hz
	
	g_pwm_cnt=TIM_TimeBaseStructure.TIM_Period;
	
	TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;						//第一次分频，简称为预分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;				//第二次分频,当前实现1分频，也就是不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);


	/* PWM1 Mode configuration: Channel1 ，让PWM的通道1工作在模式1*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//PWM模式1，在递增模式下，只要 TIMx_CNT < TIMx_CCR1，通道 1 便为有效状态（高电平），否则为无效状态（低电平）。

	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//允许输出

	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//有效的时候，输出高电平

	TIM_OC1Init(TIM3, &TIM_OCInitStructure);						//定时器3通道1初始化

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);				//定时器通道1自动重载初值，不断输出PWM脉冲

	TIM_ARRPreloadConfig(TIM3, ENABLE);							//自动重载初值使能

	/* TIM4 enable counter，使能定时器14工作 */
	TIM_Cmd(TIM3, ENABLE);
}

void sg_angle(uint32_t angle)
{
	if(angle==0)
		TIM_SetCompare1(TIM3,(uint32_t)(0.5 * g_pwm_cnt/20));

	if(angle==45)
		TIM_SetCompare1(TIM3,(uint32_t)(g_pwm_cnt/20));


	if(angle==90)
		TIM_SetCompare1(TIM3,(uint32_t)(1.5*g_pwm_cnt/20));
	
	if(angle==135)
		TIM_SetCompare1(TIM3,(uint32_t)(2*g_pwm_cnt/20));
	
	if(angle==180)
		TIM_SetCompare1(TIM3,(uint32_t)(2.5*g_pwm_cnt/20));		
}
