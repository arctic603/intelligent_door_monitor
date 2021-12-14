#include "key.h"
#include "delay.h"
#include "init_USART.h"
#include <string.h>
#include "includes.h"

static GPIO_InitTypeDef GPIO_InitStructure;

void display_key_func(void)
{
		RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));	//三个开关对应三种外部时钟的方式
		if(PAin(0) == Bit_RESET)
		{
				RCC->CFGR |= RCC_CFGR_SW_PLL;
				
		}
		
		if(PEin(2) == Bit_RESET)
		{
				RCC->CFGR |= RCC_CFGR_SW_HSE;
				
		}
		
		if(PEin(3) == Bit_RESET)
		{
				RCC->CFGR |= RCC_CFGR_SW_HSI;
				
		}

}

extern void key_init(void)
{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;							//输入模式
		GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOF,&GPIO_InitStructure);

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				
		GPIO_Init(GPIOA,&GPIO_InitStructure);
}

extern void matrix_key_init(void)
{
	//打开端口A的硬件时钟，就是对端口A供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//打开端口A的硬件时钟，就是对端口B供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	
	//打开端口A的硬件时钟，就是对端口C供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	//打开端口A的硬件时钟，就是对端口E供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	//打开端口A的硬件时钟，就是对端口G供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

					//列配置  (输出)
////////////////////////////////////////////////////////////////////////////////////	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_7;  	//PB7
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;	//输RU模式
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_DOWN;	//下拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);//初始化IO口PA0为输入
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_4;  	//PA4
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_15;  	//PG15
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_7;  	//PA4
	GPIO_Init(GPIOC,&GPIO_InitStructure);


					//行配置  (输入)
////////////////////////////////////////////////////////////////////////////////////	
	
	//GPIOF9,F10初始化设置 
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 ;		//PC9
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;			    //输入模式，
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//推挽输出，驱动LED需要电流驱动
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		    //100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;				    //上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);						//初始化GPIOF，把配置的数据写入寄存器
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 ;		//PB6
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 ;		//PE6
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 ;		//PA8
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	//列电平全部设为高电平
	PBout(7)=1;
	PAout(4)=1;
	PGout(15)=1;
	PCout(7)=1;

}

char loop_get_key_sta(void)
{
	int line;
	
	//第一列
	PBout(7)=0; 
	PAout(4)=1;
	PGout(15)=1;
	PCout(7)=1;	
	line = 1;
	while(PCin(9)==0) //(列:0,行:0)
	{
		key_stabilize(GPIOC,GPIO_Pin_9,"A",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return 'A';
	}
	while(PBin(6)==0)//(列:0,行:1)
	{
		key_stabilize(GPIOB,GPIO_Pin_6,"#",line);\
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '#';
	}		
	while(PEin(6)==0)//(列:0,行:2)
	{
		key_stabilize(GPIOE,GPIO_Pin_6,"0",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '0';			
	}		
	while(PAin(8)==0)//(列:0,行:3)
	{
		key_stabilize(GPIOA,GPIO_Pin_8,"*",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '*';
	}				
	
	//第二列
	PBout(7)=1; 
	PAout(4)=0;
	PGout(15)=1;
	PCout(7)=1;	
	line = 2;
	while(PCin(9)==0) //(列:1,行:0)
	{
		key_stabilize(GPIOC,GPIO_Pin_9,"C",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return 'C';
	}
	while(PBin(6)==0)//(列:1,行:1)
	{
		key_stabilize(GPIOB,GPIO_Pin_6,"9",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '9';
	}		
	while(PEin(6)==0)//(列:1,行:2)
	{
		key_stabilize(GPIOE,GPIO_Pin_6,"8",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '8';
	}		
	while(PAin(8)==0)//(列:1,行:3)
	{
		key_stabilize(GPIOA,GPIO_Pin_8,"7",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '7';
	}
	
	//第三列
	PBout(7)=1; 
	PAout(4)=1;
	PGout(15)=0;
	PCout(7)=1;			
	
	while(PCin(9)==0) //(列:2,行:0)
	{
		key_stabilize(GPIOC,GPIO_Pin_9,"B",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return 'B';
	}
	while(PBin(6)==0)//(列:2,行:1)
	{
		key_stabilize(GPIOB,GPIO_Pin_6,"6",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '6';	
	}		
	while(PEin(6)==0)//(列:2,行:2)
	{
		key_stabilize(GPIOE,GPIO_Pin_6,"5",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '5';	
	}		
	while(PAin(8)==0)//(列:2,行:3)
	{
		key_stabilize(GPIOA,GPIO_Pin_8,"4",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '4';	
	}		
	
	//第四列
	PBout(7)=1; 
	PAout(4)=1;
	PGout(15)=1;
	PCout(7)=0;	
	while(PCin(9)==0) //(列:3,行:0)
	{
		key_stabilize(GPIOC,GPIO_Pin_9,"D",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return 'D';
	}
	while(PBin(6)==0)//(列:3,行:1)
	{
		key_stabilize(GPIOB,GPIO_Pin_6,"3",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '3';
	}		
	while(PEin(6)==0)//(列:3,行:2)
	{
		key_stabilize(GPIOE,GPIO_Pin_6,"2",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '2';
	}		
	while(PAin(8)==0)//(列:3,行:3)
	{
		key_stabilize(GPIOA,GPIO_Pin_8,"1",line);
		PFout(8) = 1;delay_xms(200);PFout(8) = 0;
		return '1';
	}		
	
	return 'n';
}

//识别对应的按键
char get_key_status(void)
{
		char flag_menu;
		int line;
		//第一列
		PBout(7)=0; 
		PAout(4)=1;
		PGout(15)=1;
		PCout(7)=1;	
		line = 1;
		while(PCin(9)==0) //(列:0,行:0)
		{
			key_stabilize(GPIOC,GPIO_Pin_9,"A",line);
			break;
		}
		while(PBin(6)==0)//(列:0,行:1)
		{
			key_stabilize(GPIOB,GPIO_Pin_6,"#",line);
			flag_menu = '#';
			break;			
		}		
		while(PEin(6)==0)//(列:0,行:2)
		{
			key_stabilize(GPIOE,GPIO_Pin_6,"0",line);
			break;			
		}		
		while(PAin(8)==0)//(列:0,行:3)
		{
			key_stabilize(GPIOA,GPIO_Pin_8,"*",line);
			flag_menu = '*';
			break;			
		}				
		
		//第二列
		PBout(7)=1; 
		PAout(4)=0;
		PGout(15)=1;
		PCout(7)=1;	
		line = 2;
		while(PCin(9)==0) //(列:1,行:0)
		{
			key_stabilize(GPIOC,GPIO_Pin_9,"C",line);
			break;
		}
		while(PBin(6)==0)//(列:1,行:1)
		{
			key_stabilize(GPIOB,GPIO_Pin_6,"9",line);
			break;			
		}		
		while(PEin(6)==0)//(列:1,行:2)
		{
			key_stabilize(GPIOE,GPIO_Pin_6,"8",line);
			break;			
		}		
		while(PAin(8)==0)//(列:1,行:3)
		{
			key_stabilize(GPIOA,GPIO_Pin_8,"7",line);
			break;			
		}
		
		//第三列
		PBout(7)=1; 
		PAout(4)=1;
		PGout(15)=0;
		PCout(7)=1;			
		line = 3;
		while(PCin(9)==0) //(列:2,行:0)
		{
			key_stabilize(GPIOC,GPIO_Pin_9,"B",line);
			break;
		}
		while(PBin(6)==0)//(列:2,行:1)
		{
			key_stabilize(GPIOB,GPIO_Pin_6,"6",line);
			break;			
		}		
		while(PEin(6)==0)//(列:2,行:2)
		{
			key_stabilize(GPIOE,GPIO_Pin_6,"5",line);
			break;			
		}		
		while(PAin(8)==0)//(列:2,行:3)
		{
			key_stabilize(GPIOA,GPIO_Pin_8,"4",line);
			flag_menu = '4';
			break;			
		}		
		
		//第四列
		PBout(7)=1; 
		PAout(4)=1;
		PGout(15)=1;
		PCout(7)=0;	
		line = 4;
		while(PCin(9)==0) //(列:3,行:0)
		{
			key_stabilize(GPIOC,GPIO_Pin_9,"D",line);
			break;
		}
		while(PBin(6)==0)//(列:3,行:1)
		{
			key_stabilize(GPIOB,GPIO_Pin_6,"3",line);
			flag_menu = '3';
			break;			
		}		
		while(PEin(6)==0)//(列:3,行:2)
		{
			key_stabilize(GPIOE,GPIO_Pin_6,"2",line);
			flag_menu = '2';
			break;			
		}		
		while(PAin(8)==0)//(列:3,行:3)
		{
			key_stabilize(GPIOA,GPIO_Pin_8,"1",line);
			flag_menu = '1';
			break;			
		}		
		
		return flag_menu;
}


#if 1
//按键消抖
void key_stabilize(GPIO_TypeDef* GPIOx,u16 GPIO_Pin,char *ch,int line)
{

	//延时消抖
	delay_xms(50);
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) ==0)
	{
		 //等待按键1释放
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) ==0)
		{
				delay_xms(1);
		}
		
		if(GPIOx == GPIOA && GPIO_Pin == GPIO_Pin_8 && line == 1)
		{
				PFout(8) = 1;
				delay_xms(250);
				PFout(8) = 0;
				PFout(8) = 1;
				delay_xms(100);
				PFout(8) = 0;
				printf("进入功能菜单\r\n");
		}else{
				PFout(8) = 1;
				delay_xms(100);
				PFout(8) = 0;
		}
	}	
	//清空数据
	memset(ch,0,sizeof ch);	//由于所发送的ch本身只是个字符故这里简化形式写法
}
#endif

uint32_t exti_key_sta_get(void)
{
	uint32_t key_sta=0;
	
	if(PAin(0) == 0)
		key_sta|=1<<0;
	
	if(PEin(2) == 0)
		key_sta|=1<<1;
	
	if(PEin(3) == 0)
		key_sta|=1<<2;
	
	if(PEin(4) == 0)
		key_sta|=1<<3;
	
	return key_sta;
}


