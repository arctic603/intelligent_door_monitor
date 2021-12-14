#include "includes.h"

int main(void)
{		

		/*变量声明*/
		uint16_t user_total;
		
		/*模块初始化*/
		INIT_FUNC();
		delay_xms(10);
		printf("初始化模块完成\r\n");		
		
		PFout(8) = 1;
		delay_xms(50);
		PFout(8) = 0;
		
		/* 获取用户总数 */
		sfm_get_user_total(&user_total);
		printf("当前保存指纹数 = %d \r\n",user_total);
		
		OLED_Clear();
		//Display_start();
	
		while(1) 
		{	
			sg_angle(0);
			/*初始化MFRC522*/
			MFRC522_Initializtion();	
			
			if(g_rtc_wakeup_event)
			{
				
				key_sta=exti_key_sta_get();
				/*管理员模式，可添加指纹，删除指纹*/
				if(key_sta & 0x01)
				{
					printf("您已进入管理员模式，通过串口进行设置添加或者删除指纹操作\r\n");
					OLED_Clear();
					OLED_ShowCHinese(32,0,96);
					OLED_ShowCHinese(48,0,97);
					OLED_ShowCHinese(64,0,122);
					OLED_ShowCHinese(80,0,72);
					OLED_ShowCHinese(96,0,123);
					
					OLED_ShowString(16,2,"S2",16);
					OLED_ShowCHinese(44,2,124);
					OLED_ShowCHinese(60,2,125);
					OLED_ShowCHinese(76,2,8);
					OLED_ShowCHinese(92,2,9);
					
					OLED_ShowString(16,4,"S3",16);
					OLED_ShowCHinese(44,4,31);
					OLED_ShowCHinese(60,4,32);
					OLED_ShowCHinese(76,4,8);
					OLED_ShowCHinese(92,4,9);
					
					OLED_ShowString(16,6,"S4",16);
					OLED_ShowCHinese(35,6,126);
					OLED_ShowCHinese(51,6,127);
					OLED_ShowCHinese(67,6,4);
					OLED_ShowCHinese(83,6,2);
					OLED_ShowCHinese(99,6,128);
					OLED_ShowCHinese(115,6,129);
					
					while(1)
					{
						key_sta = exti_key_sta_get();
						printf("key_sta : %d\r\n",key_sta);
						key_sta_back = loop_get_key_sta();
						if(key_sta_back == '*')
						{
							OLED_Clear();
							break;
						}
						if(key_sta != 0 && key_sta != 1)
							break;

						delay_xms(200);
					}
					
					/*添加指纹*/
					if(key_sta & 0x02)
					{
						printf("当前为添加指纹模式\r\n");
						OLED_Clear();
						OLED_ShowCHinese(32,0,96);
						OLED_ShowCHinese(48,0,97);
						OLED_ShowCHinese(64,0,122);
						OLED_ShowCHinese(80,0,72);
						OLED_ShowCHinese(96,0,123);
						
						OLED_ShowCHinese(62,2,26);
						OLED_ShowCHinese(78,2,113);
						OLED_ShowCHinese(94,2,8);
						OLED_ShowCHinese(110,2,9);
						add_sfm_admin();
						
						OLED_Clear();
						OLED_ShowCHinese(32,0,96);
						OLED_ShowCHinese(48,0,97);
						OLED_ShowCHinese(64,0,122);
						OLED_ShowCHinese(80,0,72);
						OLED_ShowCHinese(96,0,123);
						
						OLED_ShowCHinese(48,2,124);
						OLED_ShowCHinese(64,2,125);
						OLED_ShowCHinese(80,2,24);
						OLED_ShowCHinese(96,2,25);
						
						display();
						delay_xms(1000);
						OLED_Clear();

					}
					
					/* 删除/获取指纹用户总数 */
					if(key_sta & 0x04)
					{
						printf("当前为删除指纹模式\r\n");
						del_sfm_admin();
						get_sfm_admin();
						
						OLED_Clear();
						OLED_ShowCHinese(32,0,96);
						OLED_ShowCHinese(48,0,97);
						OLED_ShowCHinese(64,0,122);
						OLED_ShowCHinese(80,0,72);
						OLED_ShowCHinese(96,0,123);
						
						OLED_ShowCHinese(48,2,31);
						OLED_ShowCHinese(64,2,32);
						OLED_ShowCHinese(80,2,64);
						OLED_ShowCHinese(96,2,65);
						display();
						delay_xms(1000);
						OLED_Clear();
					}
					
					/*查询解锁记录*/
					if(key_sta & 0x08)
					{
						printf("查询记录中......\r\n");
						
						/*解锁FLASH，允许操作FLASH*/
						FLASH_Unlock();

						/* 清空相应的标志位*/  
						FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
										FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 	
						
						printf("以下是解锁记录\r\n");
						//尝试获取100条记录
						for(i = 0;i < 100;i++)
						{
							//获取存储记录
							flash_read_record(buf,i);

							//检查记录是否存在换行符号，不存在则不打印输出
							if(strstr(buf,"\n") == 0)
								
								break;

							//打印记录
							printf(buf);
						}

						//如果i = 0，代表没有一条记录
						if(i == 0)
						{
							printf("没有开锁记录\r\n");
						}
						
						OLED_Clear();
						OLED_ShowCHinese(32,0,96);
						OLED_ShowCHinese(48,0,97);
						OLED_ShowCHinese(64,0,122);
						OLED_ShowCHinese(80,0,72);
						OLED_ShowCHinese(96,0,123);
						
						OLED_ShowCHinese(16,2,15);
						OLED_ShowCHinese(32,2,16);
						OLED_ShowCHinese(48,2,17);
						OLED_ShowCHinese(64,2,130);
						OLED_ShowCHinese(80,2,131);
						
						OLED_ShowCHinese(16,4,26);
						OLED_ShowCHinese(32,4,126);
						OLED_ShowCHinese(48,4,132);
						OLED_ShowCHinese(64,4,133);
						OLED_ShowCHinese(80,4,134);
						
						delay_xms(5000);
						OLED_Clear();
						
					}
						
				}
				
				Display_menu();
				if(get_key_status() == '#')
				{
					Display_func();
					if(get_key_status() == '*')
						continue;

					delay_ms(100);
				}
			
			}
			
			
			/*串口中断服务函数*/
			if(g_usart1_event)
			{
				usart_str_load(g_usart1_buf,&g_usart1_event,&g_usart1_cnt);
			}
			
			/*WIFI*/
			//检查接收到数据
			if(g_esp8266_rx_end && g_esp8266_transparent_transmission_sta)
			{
				usart_str_load(g_esp8266_rx_buf,&g_esp8266_rx_end,&g_esp8266_rx_cnt);
			}	
			
			delay_ms(100);
			
		}

}


/*USART2 串口2中断服务程序*/
void USART2_IRQHandler(void)
{
	uint8_t d=0;
	
	//检测是否接收到数据
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		d=USART_ReceiveData(USART2);
		
		g_usart2_rx_buf[g_usart2_rx_cnt++]=d;
		
		if(g_usart2_rx_cnt >= sizeof g_usart2_rx_buf)
		{
			g_usart2_rx_end=1;
		}
#if 1		
		USART_SendData(USART1,d);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
#endif		
		//清空标志位，可以响应新的中断请求
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}



void USART1_IRQHandler(void)
{
	uint8_t d=0;
	
	//检测是否接收到数据
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		d=USART_ReceiveData(USART1);
		
#if EN_DEBUG_ESP8266		
		//将接收到的数据发给串口3
		USART_SendData(USART3,d);
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);
#endif		
		//清空标志位，可以响应新的中断请求
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}



void USART3_IRQHandler(void)
{
	uint8_t d=0;
	
	//检测是否接收到数据
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		d=USART_ReceiveData(USART3);
		
		g_esp8266_rx_buf[g_esp8266_rx_cnt++]=d;
		
		if(g_esp8266_transparent_transmission_sta && d=='#')
		{
			g_esp8266_rx_end=1;
		}
		
		if(g_esp8266_rx_cnt >= sizeof g_esp8266_rx_buf)
		{
			g_esp8266_rx_end=1;
		}

#if EN_DEBUG_ESP8266		
		//将接收到的数据返发给PC
		USART_SendData(USART1,d);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
#endif		
		//清空标志位，可以响应新的中断请求
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}


/*未来可能会加入蓝牙串口设置定时闹钟开锁*/
/*RTC中断唤醒*/
void RTC_WKUP_IRQHandler(void)
{

	if(RTC_GetITStatus(RTC_IT_WUT) == SET)
	{
		g_rtc_wakeup_event=1;
		RTC_ClearITPendingBit(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line22);
	}
}


