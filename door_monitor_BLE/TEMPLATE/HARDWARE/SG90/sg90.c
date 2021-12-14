#include "sg90.h"

static GPIO_InitTypeDef 		GPIO_InitStructure;
static TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
static TIM_OCInitTypeDef  		TIM_OCInitStructure;

void sg_init(void)
{
	//ʹ�ܶ˿�Aʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	/* TIM3 clock enable ����ʱ����ʱ��ʹ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/* ����PA6 ΪPWM���ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;					//��6������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//����Ϊ���ù���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//����ģʽ��������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//����IO���ٶ�Ϊ100MHz��Ƶ��Խ������Խ�ã�Ƶ��Խ�ͣ�����Խ��
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//����Ҫ��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);

	/* Time base configuration����ʱ���Ļ������ã��������ö�ʱ������������Ƶ��Ϊ50Hz */
	TIM_TimeBaseStructure.TIM_Period = 10000/50;						//���ö�ʱ��Ƶ��Ϊ50Hz
	
	g_pwm_cnt=TIM_TimeBaseStructure.TIM_Period;
	
	TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;						//��һ�η�Ƶ�����ΪԤ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;				//�ڶ��η�Ƶ,��ǰʵ��1��Ƶ��Ҳ���ǲ���Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);


	/* PWM1 Mode configuration: Channel1 ����PWM��ͨ��1������ģʽ1*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//PWMģʽ1���ڵ���ģʽ�£�ֻҪ TIMx_CNT < TIMx_CCR1��ͨ�� 1 ��Ϊ��Ч״̬���ߵ�ƽ��������Ϊ��Ч״̬���͵�ƽ����

	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//�������

	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//��Ч��ʱ������ߵ�ƽ

	TIM_OC1Init(TIM3, &TIM_OCInitStructure);						//��ʱ��3ͨ��1��ʼ��

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);				//��ʱ��ͨ��1�Զ����س�ֵ���������PWM����

	TIM_ARRPreloadConfig(TIM3, ENABLE);							//�Զ����س�ֵʹ��

	/* TIM4 enable counter��ʹ�ܶ�ʱ��14���� */
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