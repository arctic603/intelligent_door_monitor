#include "ADC.h"

static ADC_InitTypeDef ADC_InitStructure;
static ADC_CommonInitTypeDef ADC_CommonInitStructure;
static GPIO_InitTypeDef GPIO_InitStructure;

void ADC1_INIT(void)
{
	
	//端口A的硬件时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	//ADC1的硬件时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	//配置PA5引脚为AN（模拟信号）模式
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//模拟模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//不使能上下拉电阻
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//配置ADC1相关参数
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;					//单个ADC工作
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;					//ADC1的硬件时钟频率=84MHz/2=42MHz
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 	//DMA关闭
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//5*(1/42MHz)
	ADC_CommonInit(&ADC_CommonInitStructure);	
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_8b;   					//转换结果为12位的数据
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;                               //单通道扫描
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                          //连续工作
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //不需要脉冲触发ADC工作
	//ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;						//转换的结果右对齐存储
	ADC_InitStructure.ADC_NbrOfConversion = 1;									//告诉ADC要扫描的通道数目，当前是1个
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//PA5引脚连接到ADC1，通道5序号为1，对采样点的采样时间为3个ADC时钟周期，时间越长越准确
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);
	
	//使能ADC1工作
	ADC_Cmd(ADC1, ENABLE);
	
}

void ADC3_INIT(void)
{
	
	//端口F的硬件时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	
	//ADC3的硬件时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);
	
	//配置PF7引脚为AN（模拟信号）模式
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;		//模拟模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//不使能上下拉电阻
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//配置ADC3相关参数
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;					//单个ADC工作
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;					//ADC1的硬件时钟频率=84MHz/2=42MHz
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 	//DMA关闭
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//5*(1/42MHz)
	ADC_CommonInit(&ADC_CommonInitStructure);	
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;   					//转换结果为12位的数据
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;                               //单通道扫描
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                          //连续工作
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //不需要脉冲触发ADC工作
	//ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;						//转换的结果右对齐存储
	ADC_InitStructure.ADC_NbrOfConversion = 1;									//告诉ADC要扫描的通道数目，当前是1个
	ADC_Init(ADC3, &ADC_InitStructure);
	
	//PF7引脚连接到ADC3，通道5序号为1，对采样点的采样时间为3个ADC时钟周期，时间越长越准确
	ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);
	
	//使能ADC3工作
	ADC_Cmd(ADC3, ENABLE);



}
