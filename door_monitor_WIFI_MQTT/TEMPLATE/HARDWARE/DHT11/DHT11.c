#include "DHT11.h"
#define PGout(y) (*(volatile uint32_t *)(0x42000000 + (GPIOG_BASE + 0x14 - 0x40000000)*32 + y*4))
#define PGin(x)	 (*(volatile uint32_t *)(0x42000000 + (GPIOG_BASE + 0x10 - 0x40000000)*32 + x*4))
static GPIO_InitTypeDef GPIO_InitStructure;

void DHT11_INIT(uint8_t op)
{
	//打开端口G的硬件时钟，就是供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9; 	//9号引脚
	if(op)
	{
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;	//输出模式
	}else{
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;	//输出模式
	}
	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;	//开漏
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;//高速，速度越高，响应越快，但是功耗会更高
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;//不使能上下拉电阻
	GPIO_Init(GPIOG,&GPIO_InitStructure);

	//只要有输出模式，肯定会有初始电平的状态，看连接设备的说明书
	PGout(9)=1;
}

void DHT11_OUTPUT_MODE(void)	//用于初始化发送信号给模块等待其响应
{
	PGout(9)=0;
	delay_xms(18);
	
	PGout(9)=1;
	delay_xus(30);
}

int16_t DHT11_REC_DATA(uint8_t *array)
{
	DHT11_INIT(1);	//默认初始化模块为输出引脚
	
	DHT11_OUTPUT_MODE();
	
	DHT11_INIT(0);	//开始接收模块响应转换为输入引脚
	
	uint32_t t=0;
	int32_t i=0,j=0;
	uint8_t d=0;
	uint8_t *p= array;
	uint8_t check_sum=0;
	
	//等待低电平出现
	t=0;
	while(PGin(9))
	{
		t++;
		delay_xus(1);
		
		if(t >= 4000)
			return -1;
	}

	//用超时检测的方法测量低电平的合法性
	t=0;
	while(PGin(9)==0)
	{
		t++;
		delay_xus(1);
		
		if(t >= 100)
			return -2;
	}	
	
	//用超时检测的方法测量高电平的合法性
	t=0;
	while(PGin(9))
	{
		t++;
		delay_xus(1);
		
		if(t >= 100)
			return -3;
	}	
	
	for(j=0; j<5; j++)
	{
		//接收一个字节的数据
		for(d=0,i=7; i>=0; i--)
		{
			//用超时检测的方法测量低电平的合法性
			t=0;
			while(PGin(9)==0)
			{
				t++;
				delay_xus(1);
				
				if(t >= 100)
					return -4;
			}

			//延时40us （延时时间在28us ~ 70us）
			delay_xus(40);
			
			if(PGin(9))
			{
				d|=1<<i;	//将d变量对应的bit置1
				
				//等待高电平持续完毕
				t=0;
				while(PGin(9))
				{
					t++;
					delay_xus(1);
					
					if(t >= 100)
						return -5;
				}			
			}
		}	
		p[j]=d;
	}
	
	//延时50us，可以忽略通讯结束的低电平
	delay_xus(50);
	
	//计算校验和，检查接收到的数据是否准确
	check_sum = (p[0]+p[1]+p[2]+p[3])&0xFF;
	
	if(check_sum == p[4])
		return 0;
	
	return -6;

}
