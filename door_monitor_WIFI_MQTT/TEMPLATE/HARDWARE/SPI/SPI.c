#include "SPI.h"

void W25Q128_INIT(void)
{
	//使能端口B硬件时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//使能SPI1硬件时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	//配置PB3~PB5为复用功能模式
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3 | GPIO_Pin_4|GPIO_Pin_5; 	//3 4 5号引脚
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;//高速，速度越高，响应越快，但是功耗会更高
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;//不使能上下拉电阻
	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	
	//将PB3~PB5连接到SPI1硬件
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);


	//配置PB14为输出模式
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14 ; 	//14号引脚
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Low_Speed;//高速，速度越高，响应越快，但是功耗会更高
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;//不使能上下拉电阻
	GPIO_Init(GPIOB,&GPIO_InitStructure);		
	
	//只要是输出模式，要给予它初始电平状态
	PBout(14)=1;
	
	//初始化SPI1相关参数
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //全双工通信
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	//主机角色
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;				//看从机的数据手册，通信数据每次大小为8bit
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;						//看从机的数据手册，CPOL=1
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;					//看从机的数据手册，CPHA=1 ，目前为模式3
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;						//看从机的数据手册，片选引脚由软件代码控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;//看从机的数据手册，SPI的时钟=84MHz/16=5.25MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				//看从机的数据手册
	//SPI_InitStructure.SPI_CRCPolynomial = 7;						//这个是用于两个M4进行通信，spi flash不支持
	SPI_Init(SPI1, &SPI_InitStructure);
 	
	
	//使能SPI1硬件工作
	SPI_Cmd(SPI1,ENABLE);
}


uint8_t SPI1_SendByte(uint8_t byte)
{
	/*!< Loop while DR register in not full,??????????????? */
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);
	
	/*!< Send byte through the SPI1 peripheral?????SPI1?????????? */
	SPI_I2S_SendData(SPI1,byte);
	
	/*!< Wait to receive a byte?????????????????????SPI?????? */
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET);
	
	/*!< Return the byte read from the SPI bus ???????????????*/
	return SPI_I2S_ReceiveData(SPI1);
	
}

void W25Q128_read_id(uint8_t *m_id,uint8_t *d_id)
{
	//从机工作有效（从机开始工作）
	W25Q128_CS=0;

	//发送0x90指令，命令SPI FLASH要提供ID信息
	SPI1_SendByte(0x90);
	
	//24bit-address全为0
	SPI1_SendByte(0x00);	
	SPI1_SendByte(0x00);
	SPI1_SendByte(0x00);
	
	/* 任意参数，参数随意 */
	*m_id=SPI1_SendByte(0x8);

	/* 任意参数，参数随意 */
	*d_id=SPI1_SendByte(0x4);

	//通讯完毕，从机工作无效（从机停止工作）
	W25Q128_CS=1;	
}

/*??????*/
void W25Q128_read_data(uint32_t addr,uint8_t *buf,uint32_t len)
{
	uint8_t *p=buf;
	
	//从机工作有效（从机开始工作）
	W25Q128_CS=0;	

	//发送0x03指令，命令SPI FLASH要提供内部存储数据
	SPI1_SendByte(0x03);
	
	//addr = 0x123456
	SPI1_SendByte((addr>>16)&0xFF);//addr>>16 0x123456>>16 &0xFF = 0x12
	SPI1_SendByte((addr>>8) &0xFF);//addr>>8  0x123456>>8  &0xFF = 0x34
	SPI1_SendByte( addr     &0xFF);//addr>>0  0x123456>>0  &0xFF = 0x56


	while(len--)
	/* 任意参数，参数随意 */
		*p++=SPI1_SendByte(0xFF);

	//通讯完毕，从机工作无效（从机停止工作）
	W25Q128_CS=1;	
}

/*解除写保护*/
void W25Q128_write_enable(void)
{
	//从机工作有效（从机开始工作）
	W25Q128_CS=0;	

	//发送0x06指令，命令SPI FLASH要解除写保护
	SPI1_SendByte(0x06);
	
	
	//通讯完毕，从机工作无效（从机停止工作）
	W25Q128_CS=1;
}

/*打开写保护*/
void W25Q128_write_disable(void)
{
	//从机工作有效（从机开始工作）
	W25Q128_CS=0;	

	//发送0x04指令，命令SPI FLASH要开启写保护
	SPI1_SendByte(0x04);
	
	//通讯完毕，从机工作无效（从机停止工作）
	W25Q128_CS=1;

}

uint8_t W25Q128_read_status_1(void)
{
	uint8_t status = 0;
	
	//从机工作有效
	W25Q128_CS = 0;
	
	//发送0x05指令，命令SPI_FLASH要提供状态寄存器1信息
	SPI1_SendByte(0x05);
	
	status = SPI1_SendByte(0xFF);
	
	//通讯完毕，从机工作无效（从机停止工作）
	W25Q128_CS = 1;
	
	return status;

}

/*擦除*/
void W25Q128_erase_sector(uint32_t addr)
{
	uint8_t status = 0;
	
	//执行写使能指令
	W25Q128_write_enable();
	
	//延时一会儿，让片选引脚能够给保持高电平一段时间，让w25qxx识别
	delay_xus(10);
	
	//片选引脚拉低
	W25Q128_CS = 0;
	
	//发送0x20
	SPI1_SendByte(0x20);
	
	//发送24bit的地址
	SPI1_SendByte((addr >> 16) & 0xFF);
	SPI1_SendByte((addr >> 8) & 0xFF);
	SPI1_SendByte( addr & 0xFF);
	
	//片选引脚拉高
	W25Q128_CS = 1;
	
	//延时一会儿,让片选引脚能够保持高电平一段时间，让W25Q128识别
	delay_xus(10);
	
	//检查当前是否已经擦除成功
	while(1)
	{
		status = W25Q128_read_status_1();
		
		if((status & 0x01) == 0)
			break;
		
		//每隔1ms进行查询
		delay_xms(1);
	}
	
	//开启写保护
	W25Q128_write_disable();

}

void W25Q128_write_page(uint32_t addr,uint8_t *pbuf,uint32_t len)
{
	uint8_t status = 0;
	
	//执行写使能指令
	W25Q128_write_enable();
	
	//延时一会儿，让片选引脚能够给保持高电平一段时间，让W25QXX识别
	delay_xus(10);
	
	//片选引脚拉低
	W25Q128_CS = 0;

	//发送0x02
	SPI1_SendByte(0x02);

	//发送24bit的地址
	SPI1_SendByte((addr>>16)&0xFF);
	SPI1_SendByte((addr>>8)&0xFF);
	SPI1_SendByte( addr&0xFF);
	
	//发送数据
	while(len--)
		SPI1_SendByte(*pbuf++);

	//片选引脚拉高	
	W25Q128_CS=1;
	
	//延时一会儿，让片选引脚能够给保持高电平一段时间，让w25qxx识别
	delay_xus(10);
	
	
	//检查当前是否已经写入成功
	while(1)
	{
		status=	W25Q128_read_status_1();
		
		if((status & 0x01) ==0)
			break;
	
		//每隔1毫秒进行查询
		delay_xms(1);
	
	}
	
	//开启写保护
	W25Q128_write_disable();
}

void W25Q128_INIT_MODE(uint8_t ch)
{
	//使能端口B的硬件时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

	//配置端口B PB3 PB5 PB14位输出模式
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;	
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;									
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//配置端口B PB4 
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;	
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;									
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//看时序图，只要是输出模式,初始状态要对应相应电平
	W25Q128_CS = 1;		//SS引脚
	if(ch == 0)
	W25Q128_SCLK = 0; 	//SCLK引脚
	else if(ch == 3)
	W25Q128_SCLK = 1;

	
	W25Q128_MOSI = 1;	//MOSI引脚

}


uint8_t SPI1_SendByte_MODE0(uint8_t byte)
{
	int32_t i = 0;
	uint8_t d = 0;

	for(i = 7;i >= 0;i--)
	{
		//MSB,最高有效位发送数据
		if(byte & (1<<i))
			W25Q128_MOSI = 1;
		else	
			W25Q128_MOSI = 0;		

		//设置SCLK引脚为高电平,告诉从机，我开始采集MISO引脚电平
		W25Q128_SCLK = 1;
		delay_xus(2);

		//主机采集MISO引脚电平
		if(W25Q128_MISO)
			d |= 1<<i;

		//设置SCLK引脚为低电平，这时，从机开始读取MOSI电平
		W25Q128_SCLK = 0;
		delay_xus(2);
	}

	return d;
}

uint8_t SPI1_SendByte_MODE3(uint8_t byte)
{
	int32_t i = 0;
	uint8_t d = 0;

	for(i = 7;i >= 0;i--)
	{
		//MSB,最高有效位发送数据
		if(byte & (1<<i))
			W25Q128_MOSI = 1;
		else
			W25Q128_MOSI = 0;	
		
		//设置SCLK引脚为低电平
		W25Q128_SCLK = 0;
		delay_xus(2);

		//主机采集MISO引脚电平
		if(W25Q128_MISO)
			d|=1<<i;

	}

	return d;
	
}
