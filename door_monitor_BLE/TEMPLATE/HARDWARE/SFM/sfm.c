#include "stm32f4xx.h"
#include "delay.h"
#include "init_USART.h"
#include "sfm.h"
#include "beep.h"
#include "sys.h"

#include <string.h>
#include <stdio.h>

void delay_ms(uint32_t n)
{
	delay_xms(n);
}

uint8_t bcc_check(uint8_t *buf,uint32_t len)
{
	uint8_t s=0;
	uint8_t i=0;
	uint8_t *p = buf;
	
	for(i=0; i<len; i++)
		s = s^p[i];

	return s;
}

void sfm_touch_init(void)
{
	GPIO_InitTypeDef 		GPIO_InitStructure;

	//打开端口A的硬件时钟，就是对端口A供电
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	//配置GPIOA的第8根引脚
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	
	//初始化
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
}

uint32_t sfm_touch_sta(void)
{
	return (PAin(8)==0);
}

int32_t sfm_init(uint32_t baud)
{
	int32_t rt=0;
	/* sfm触摸检测引脚初始化 */
	sfm_touch_init();

	/* 光圈控制:全亮->全灭，周期2秒 */
	rt= sfm_ctrl_led(0x00,0x07,0xC8);
	if(rt != SFM_ACK_SUCCESS)	
		return rt;
	return SFM_ACK_SUCCESS;
}


int32_t sfm_ctrl_led(uint8_t led_start,uint8_t led_end,uint8_t period)
{
	uint8_t buf_tx[8]={0};
	
	memset((void *)g_usart2_rx_buf,0,sizeof g_usart2_rx_buf);
	g_usart2_rx_cnt=0;
	g_usart2_rx_end=0;
	
	/* 发送设置光圈控制命令 */
	buf_tx[0]=0xF5;
	buf_tx[1]=0xC3;	
	buf_tx[2]=led_start;	
	buf_tx[3]=led_end;	
	buf_tx[4]=period;		
	buf_tx[5]=0;
	buf_tx[6]=bcc_check(&buf_tx[1],5);
	buf_tx[7]=0xF5;
	
	usart_send_bytes(USART2,buf_tx,8);
	
//	delay_xus(1000);
	delay_ms(1000);
	
	if((g_usart2_rx_buf[0] == 0xF5) && (g_usart2_rx_buf[1] == 0xC3)&& (g_usart2_rx_buf[2] == SFM_ACK_SUCCESS))
	{
		return SFM_ACK_SUCCESS;
	}
	
	return SFM_ACK_FAIL;
}

int32_t sfm_reg_user(uint16_t id)
{
	uint8_t buf_tx[8]={0};
	
	memset((void *)g_usart2_rx_buf,0,sizeof g_usart2_rx_buf);
	g_usart2_rx_cnt=0;
	g_usart2_rx_end=0;
	
	/* 注册用户命令 */
	buf_tx[0]=0xF5;
	buf_tx[1]=0x01;	
	buf_tx[2]=(uint8_t)(id>>8);	
	buf_tx[3]=(uint8_t)(id&0x00FF);	
	buf_tx[4]=0x01;		
	buf_tx[5]=0x00;
	buf_tx[6]=bcc_check(&buf_tx[1],5);
	buf_tx[7]=0xF5;
	
	usart_send_bytes(USART2,buf_tx,8);
	
	delay_ms(1000);
	
	if(!((g_usart2_rx_buf[0] == 0xF5) && (g_usart2_rx_buf[1] == 0x01) && (g_usart2_rx_buf[4] == SFM_ACK_SUCCESS)))
	{
		return g_usart2_rx_buf[4];
	}
	
	memset((void *)g_usart2_rx_buf,0,sizeof g_usart2_rx_buf);
	g_usart2_rx_cnt=0;
	g_usart2_rx_end=0;
	
	/* 发送第2次 */
	buf_tx[0]=0xF5;
	buf_tx[1]=0x02;	
	buf_tx[2]=0x00;	
	buf_tx[3]=0x00;	
	buf_tx[4]=0x00;		
	buf_tx[5]=0x00;
	buf_tx[6]=bcc_check(&buf_tx[1],5);
	buf_tx[7]=0xF5;
	
	usart_send_bytes(USART2,buf_tx,8);
	
	delay_ms(1000);
	
	if(!((g_usart2_rx_buf[0] == 0xF5) && (g_usart2_rx_buf[1] == 0x02) && (g_usart2_rx_buf[4] == SFM_ACK_SUCCESS)))
	{
		return g_usart2_rx_buf[4];
	}	
	
	memset((void *)g_usart2_rx_buf,0,sizeof g_usart2_rx_buf);
	g_usart2_rx_cnt=0;
	g_usart2_rx_end=0;	
	

	/* 发送第3次 */
	buf_tx[0]=0xF5;
	buf_tx[1]=0x03;	
	buf_tx[2]=0x00;	
	buf_tx[3]=0x00;	
	buf_tx[4]=0x00;		
	buf_tx[5]=0x00;
	buf_tx[6]=bcc_check(&buf_tx[1],5);
	buf_tx[7]=0xF5;
	
	usart_send_bytes(USART2,buf_tx,8);
	
	delay_ms(1000);
	
	if(!((g_usart2_rx_buf[0] == 0xF5) && (g_usart2_rx_buf[1] == 0x03) && (g_usart2_rx_buf[4] == SFM_ACK_SUCCESS)))
	{
		return g_usart2_rx_buf[4];
	}
	
	if(g_usart2_rx_buf[2] == ((uint8_t)(id>>8)))
		if(g_usart2_rx_buf[3] == ((uint8_t)(id&0x00FF)))
			return SFM_ACK_SUCCESS;
		
	return g_usart2_rx_buf[2];
}

int32_t sfm_compare_users(uint16_t *id)
{
	uint8_t buf_tx[8]={0};
	
	memset((void *)g_usart2_rx_buf,0,sizeof g_usart2_rx_buf);
	g_usart2_rx_cnt=0;
	g_usart2_rx_end=0;
	
	/* 1:N比对 */
	buf_tx[0]=0xF5;
	buf_tx[1]=0x0C;	
	buf_tx[2]=0x00;	
	buf_tx[3]=0x00;	
	buf_tx[4]=0x00;		
	buf_tx[5]=0x00;
	buf_tx[6]=bcc_check(&buf_tx[1],5);
	buf_tx[7]=0xF5;
	
	usart_send_bytes(USART2,buf_tx,8);
	
	delay_ms(1000);
	
	if(!((g_usart2_rx_buf[0] == 0xF5) && (g_usart2_rx_buf[1] == 0x0C)))
	{
		return SFM_ACK_FAIL;
	}	
	
	/* 返回用户id */
	*id = (g_usart2_rx_buf[2]<<8)|g_usart2_rx_buf[3];
	
	/* 若id = 0x0000，表示比对不成功 */
	if(*id == 0x0000)
		return SFM_ACK_NOUSER;
	
	return SFM_ACK_SUCCESS;
}


int32_t sfm_get_unused_id(uint16_t *id)
{
	uint8_t buf_tx[8]={0};
	
	memset((void *)g_usart2_rx_buf,0,sizeof g_usart2_rx_buf);
	g_usart2_rx_cnt=0;
	g_usart2_rx_end=0;
	
	/* 获取未使用的用户命令 */
	buf_tx[0]=0xF5;
	buf_tx[1]=0x0D;	
	buf_tx[2]=0x00;	
	buf_tx[3]=0x00;	
	buf_tx[4]=0x00;		
	buf_tx[5]=0x00;
	buf_tx[6]=bcc_check(&buf_tx[1],5);
	buf_tx[7]=0xF5;
	
	usart_send_bytes(USART2,buf_tx,8);
	
	delay_ms(1000);
	
	*id = (g_usart2_rx_buf[2]<<8)|g_usart2_rx_buf[3];
	
	if((g_usart2_rx_buf[0] == 0xF5) && (g_usart2_rx_buf[1] == 0x0D) && (g_usart2_rx_buf[4] == SFM_ACK_SUCCESS))
	{
		return SFM_ACK_SUCCESS;
	}

	return SFM_ACK_FAIL;
}

int32_t sfm_del_user(uint16_t id)
{
	uint8_t buf_tx[8]={0};
	
	memset((void *)g_usart2_rx_buf,0,sizeof g_usart2_rx_buf);
	g_usart2_rx_cnt=0;
	g_usart2_rx_end=0;
	
	/* 删除用户命令 */
	buf_tx[0]=0xF5;
	buf_tx[1]=0x04;	
	buf_tx[2]=(uint8_t)(id>>8);	
	buf_tx[3]=(uint8_t)(id&0x00FF);	
	buf_tx[4]=0x01;		
	buf_tx[5]=0x00;
	buf_tx[6]=bcc_check(&buf_tx[1],5);
	buf_tx[7]=0xF5;
	
	usart_send_bytes(USART2,buf_tx,8);
	
	delay_ms(1000);
	
	if((g_usart2_rx_buf[0] == 0xF5) && (g_usart2_rx_buf[1] == 0x04) && (g_usart2_rx_buf[4] == SFM_ACK_SUCCESS))
	{
		return SFM_ACK_SUCCESS;
	}

	return SFM_ACK_FAIL;
}


int32_t sfm_del_user_all(void)
{
	uint8_t buf_tx[8]={0};
	
	memset((void *)g_usart2_rx_buf,0,sizeof g_usart2_rx_buf);
	g_usart2_rx_cnt=0;
	g_usart2_rx_end=0;
	
	/* 删除所有用户命令 */
	buf_tx[0]=0xF5;
	buf_tx[1]=0x05;	
	buf_tx[2]=0x00;	
	buf_tx[3]=0x00;	
	buf_tx[4]=0x00;		
	buf_tx[5]=0x00;
	buf_tx[6]=bcc_check(&buf_tx[1],5);
	buf_tx[7]=0xF5;
	
	usart_send_bytes(USART2,buf_tx,8);
	
	delay_ms(1000);
	
	if((g_usart2_rx_buf[0] == 0xF5) && (g_usart2_rx_buf[1] == 0x05) && (g_usart2_rx_buf[4] == SFM_ACK_SUCCESS))
	{
		return SFM_ACK_SUCCESS;
	}

	return SFM_ACK_FAIL;
}

int32_t sfm_get_user_total(uint16_t *user_total)
{
	uint8_t buf_tx[8]={0};
	
	memset((void *)g_usart2_rx_buf,0,sizeof g_usart2_rx_buf);
	g_usart2_rx_cnt=0;
	g_usart2_rx_end=0;
	
	/* 删除用户命令 */
	buf_tx[0]=0xF5;
	buf_tx[1]=0x09;	
	buf_tx[2]=0x00;	
	buf_tx[3]=0x00;	
	buf_tx[4]=0x00;		
	buf_tx[5]=0x00;
	buf_tx[6]=bcc_check(&buf_tx[1],5);
	buf_tx[7]=0xF5;
	
	usart_send_bytes(USART2,buf_tx,8);
	
	delay_ms(1000);
	
	if(!((g_usart2_rx_buf[0] == 0xF5) && (g_usart2_rx_buf[1] == 0x09)))
	{
		return SFM_ACK_FAIL;
	}	
	
	/* 返回用户总数 */
	*user_total = (g_usart2_rx_buf[2]<<8)|g_usart2_rx_buf[3];
	
	return SFM_ACK_SUCCESS;
}

int32_t sfm_touch_check(void)
{
	uint8_t buf_tx[8]={0};
	
	memset((void *)g_usart2_rx_buf,0,sizeof g_usart2_rx_buf);
	g_usart2_rx_cnt=0;
	g_usart2_rx_end=0;
	
	/* 检测触摸感应命令 */
	buf_tx[0]=0xF5;
	buf_tx[1]=0x30;	
	buf_tx[2]=0x00;	
	buf_tx[3]=0x00;	
	buf_tx[4]=0x00;		
	buf_tx[5]=0x00;
	buf_tx[6]=bcc_check(&buf_tx[1],5);
	buf_tx[7]=0xF5;
	
	usart_send_bytes(USART2,buf_tx,8);
	
	delay_ms(1000);
	
	if((g_usart2_rx_buf[0] == 0xF5) && (g_usart2_rx_buf[1] == 0x30) && (g_usart2_rx_buf[4] == SFM_ACK_SUCCESS))
	{
		return SFM_ACK_SUCCESS;
	}	
	
	return SFM_ACK_FAIL;
}

const char *sfm_error_code(uint8_t error_code)
{
	const char *p;
	
	switch(error_code)
	{
		case SFM_ACK_SUCCESS:p="执行成功";
		break;
		
		case SFM_ACK_FAIL:p="执行失败";
		break;	

		case SFM_ACK_FULL:p="数据库满";
		break;		

		case SFM_ACK_NOUSER:p="没有这个用户";
		break;		

		case SFM_ACK_USER_EXIST:p="用户已存在";
		break;	
		
		case SFM_ACK_TIMEOUT:p="图像采集超时";
		break;		
	
		case SFM_ACK_HARDWAREERROR:p="硬件错误";
		break;	
		
		case SFM_ACK_IMAGEERROR:p="图像错误";
		break;	

		case SFM_ACK_BREAK:p="终止当前指令";
		break;	

		case SFM_ACK_ALGORITHMFAIL:p="贴膜攻击检测";
		break;	
		
		case SFM_ACK_HOMOLOGYFAIL:p="同源性校验错误";
		break;

		default :
			p="模块返回确认码有误";break;
	}

	return p;


}
