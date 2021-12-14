#include "includes.h"
#include "bmp.h"
#include "PAJ7620.h"

/*功能选择标志宏*/
#define SFM_UNLOCK '1'
#define PASSWORD_UNLOCK '2'
#define RFID_UNLOCK '3'
#define HAND_STATE_UNLOCK '4'
#define GESTURE_SIZE sizeof(gesture_arry)/2

/*默认初始化开锁密码*/
static const char password[] = "603328";

//手势识别初始化数组
static const unsigned char gesture_arry[][2]={
	
	{0xEF,0x00},
	{0x41,0x00},
	{0x42,0x00},
	{0xEF,0x00},
	{0x48,0x3C},
	{0x49,0x00},
	{0x51,0x10},
	{0x83,0x20},
	{0x9F,0xF9},
	{0xEF,0x01},
	{0x01,0x1E},
	{0x02,0x0F},
	{0x03,0x10},
	{0x04,0x02},
	{0x41,0x40},
	{0x43,0x30},
	{0x65,0x96},
	{0x66,0x00},
	{0x67,0x97},
	{0x68,0x01},
	{0x69,0xCD},
	{0x6A,0x01},
	{0x6B,0xB0},
	{0x6C,0x04},
	{0x6D,0x2C},
	{0x6E,0x01},
	{0x74,0x00},
	{0xEF,0x00},
	{0x41,0xFF},
	{0x42,0x01},

};

/*LED闪烁加蜂鸣器警报*/
void display(void)
{
		PFout(9) = PFout(10) = PEout(13) = PEout(14) = 0;
		delay_xms(800);
		PFout(9) = PFout(10) = PEout(13) = PEout(14) = 1;
		
		PFout(9) = PFout(10) = PEout(13) = PEout(14) = 0;
		delay_xms(800);
		PFout(9) = PFout(10) = PEout(13) = PEout(14) = 1;
		
		/**/
		PFout(8) = 1;delay_xms(300);PFout(8) = 0;
		PFout(8) = 1;delay_xms(300);PFout(8) = 0;

}

/*管理员模式下的指纹操作函数*/

//添加指纹
void add_sfm_admin(void)
{
		int32_t rt;
		uint16_t id=1;
		uint32_t timeout=0;
		
		printf("\r\n\r\n=====================================\r\n\r\n");
		printf("执行添加指纹操作,请将手指放到指纹模块触摸感应区\r\n");
		timeout=0;
			
		/* 显示蓝色 */
		sfm_ctrl_led(0x06,0x06,0x32);
			
		while((sfm_touch_check()!=SFM_ACK_SUCCESS) && (timeout<10))
		{
			timeout++;
		}
			
		if(timeout>=10)
		{
			printf("没有检测到手指，请重新操作!\r\n");
			
			/* 恢复光圈全亮->全灭，周期2秒 */
			sfm_ctrl_led(0x00,0x07,0xC8);
			
			return ;
		}
			
		printf("检测到手指，现在开始添加指纹...\r\n");
			
		/* 获取未使用的用户id */
		rt = sfm_get_unused_id(&id);
			
		if(rt != SFM_ACK_SUCCESS)
		{
			printf("获取未使用的用户id %s\r\n",sfm_error_code(rt));
			
			/* 恢复光圈全亮->全灭，周期2秒 */
			sfm_ctrl_led(0x00,0x07,0xC8);				
			
			return ;
		}
		
		printf("将使用的用户id为%d\r\n",id);	
			
		rt=sfm_reg_user(id);
		
		if(rt == SFM_ACK_SUCCESS)
		{
			/* 成功:光圈显示绿色 */
			sfm_ctrl_led(0x05,0x05,0x32);			
			delay_ms(1000);
			
			/* 成功，蜂鸣器嘀一声示意 */
			PFout(8) = 1;delay_ms(50);PFout(8) = 0;					
			
		}
		else
		{
			/* 失败:光圈显示红色 */
			sfm_ctrl_led(0x03,0x03,0x32);
			
			delay_ms(1000);			
		}
		
		
		printf("添加指纹 %s\r\n",sfm_error_code(rt));
		
		/* 恢复光圈全亮->全灭，周期2秒 */
		sfm_ctrl_led(0x00,0x07,0xC8);		

}


//获取指纹
void get_sfm_admin(void)
{
		int32_t rt;
		uint16_t user_total;
		printf("\r\n\r\n=====================================\r\n\r\n");

		rt=sfm_get_user_total(&user_total);

		printf("电容指纹模块用户总数 %s %d \r\n",sfm_error_code(rt),user_total);
	
		if(rt == SFM_ACK_SUCCESS)
		{
			/* 成功，蜂鸣器嘀一声示意 */
			PFout(8) = 1;delay_xms(50);PFout(8) = 0;
		}		

}

//删除指纹
void del_sfm_admin(void)
{
		int32_t rt;
		printf("\r\n\r\n=====================================\r\n\r\n");

		rt = sfm_del_user_all();

		printf("删除所有用户 %s\r\n",sfm_error_code(rt));
		
		if(rt == SFM_ACK_SUCCESS)
		{
			/* 成功，蜂鸣器嘀一声示意 */
			PFout(8) = 1;delay_xms(50);PFout(8) = 0;			
		}
		
		printf("删除用户成功\r\n");			
}

/*解锁数据记录*/
int search_unlock_record_admin(int lock_chose,int lock_character)
{
	//获取日期
	RTC_GetDate(RTC_Format_BCD,&RTC_DateStructure);
	//获取时间
	RTC_GetTime(RTC_Format_BCD,&RTC_TimeStructure);

	//检查当前数据是否小于100条
	if(dht11_rec_cnt < 100)
	{
		//获取日期
		RTC_GetDate(RTC_Format_BCD,&RTC_DateStructure);

		//获取时间
		RTC_GetTime(RTC_Format_BCD,&RTC_TimeStructure);

		//格式化字符串,末尾添加\r\n作为一个结束标记，方便我们读取时候进行判断
		sprintf((char *)buf, "[%03d]20%02x/%02x/%02x %02x:%02x:%02x %s %s\r\n", \
				dht11_rec_cnt,\
				RTC_DateStructure.RTC_Year,RTC_DateStructure.RTC_Month,RTC_DateStructure.RTC_Date,\
				RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds,\
				unlock_record_npc[lock_character],lock_func_chose[lock_chose]);
		
		/*解锁FLASH，允许操作FLASH*/
		FLASH_Unlock();

		/* 清空相应的标志位*/  
		FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
						FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 	
		
		//写入解锁记录
		if(0 == flash_write_record(buf,dht11_rec_cnt))
		{
			//显示
			printf(buf);

			//记录自加1
			dht11_rec_cnt++;
		}else{

			//数据记录清零,重头开始存储数据
			dht11_rec_cnt = 0;
		}
		
	}else{
		printf("已超过100条解锁记录!需清理\r\n");
		
	}
		
	
	return 0;
}



/*主函数模块初始化*/
void INIT_FUNC(void)
{
	int rt;
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	Init_LED();
	beep_init();
	
	/*初始化串口1*/
	usart1_init(115200);
	
	/* 串口2 ==>指纹初始化 */
	usart2_init(115200);
	
	/*初始化MFRC522*/
	MFRC522_Initializtion();	
	
	/*温湿度*/
	DHT11_INIT(1);
	OLED_Init();
	OLED_Clear();
	delay_xms(500);
	
	//串口延迟一会，确保芯片内部完成全部初始化,printf无乱码输出
	delay_ms(500);
		
	//发送数据
	printf("This is esp8266 mqtt with aliyun test by teacher.wen\r\n");
	
	while(esp8266_mqtt_init())
	{
		printf("esp8266_mqtt_init ...");
		
		delay_ms(1000);
	}
	
	//蜂鸣器嘀两声，D1灯闪烁两次，示意连接成功
	 PFout(9)=0;delay_ms(100);
	 PFout(9)=1;delay_ms(100);	
	 PFout(9)=0;delay_ms(100);
	 PFout(9)=1;delay_ms(100);

	printf("esp8266 connect aliyun with mqtt success\r\n");	
	
	/*矩阵键盘*/
	matrix_key_init();
	key_init();
	printf("矩阵键盘初始化正常\r\n");
	
	/*舵机模拟门锁初始化*/
	sg_init();
	sg_angle(45);
	printf("舵机初始化正常\r\n");
	
	/* 跟电容指纹模块进行握手 */
	while(SFM_ACK_SUCCESS!=sfm_init(115200))
	{
		delay_ms(500);
	
		printf("电容指纹模块连接中 ...\r\n");		
	}
	printf("电容指纹模块已连接上\r\n");
	
	//PAJ7620U2传感器初始化
 	while(!paj7620u2_init())
	{
	    printf("手势识别初始化异常!!!\r\n");
		delay_ms(500);
	}
	
    printf("手势识别初始化成功 OK\r\n");
	RTC_RST_CONFIG();
}

/*	开机动画显示  */
void Display_start(void)
{
		uint8_t date[64] = {0};
		
		//获取日期
		RTC_GetDate(RTC_Format_BCD,&RTC_DateStructure);
		sprintf((char *)date,"20%02x/%02x/%02x",
				RTC_DateStructure.RTC_Year,
				RTC_DateStructure.RTC_Month,
				RTC_DateStructure.RTC_Date);
		printf("\r\n");

		//开机动画
		OLED_Clear();//关闭显示
		OLED_ShowCHinese(32,0,37);//欢
		OLED_ShowCHinese(50,0,38);//迎
		OLED_ShowCHinese(68,0,39);//使
		OLED_ShowCHinese(86,0,40);//用	
		
		OLED_ShowCHinese(23,2,6);//智
		OLED_ShowCHinese(41,2,7);//能
		OLED_ShowCHinese(59,2,8);//指
		OLED_ShowCHinese(77,2,9);//纹
		OLED_ShowCHinese(95,2,5);//锁
		
		OLED_ShowCHinese(38,4,10);//孙
		OLED_ShowCHinese(56,4,11);//懿
		OLED_ShowCHinese(74,4,12);//轩
		
		OLED_ShowString(20,6,date,16);
		memset(date,0,sizeof date);
		
		delay_ms(3000);
		OLED_Clear();//关闭显示
		OLED_DrawBMP(0,0,128,8,start_init);//开机外星人图标初始
		delay_ms(700);		
		//外星人进度条
		OLED_DrawBMP(0,0,128,8,start_sta1);//开机外星人图标0
		delay_ms(600);
		OLED_DrawBMP(0,0,128,8,start_sta2);//开机外星人图标1	
		delay_ms(600);
		OLED_DrawBMP(0,0,128,8,start_sta3);//开机外星人图标2	
		delay_ms(600);
		OLED_DrawBMP(0,0,128,8,start_sta4);//开机外星人图标3
		delay_ms(600);
		OLED_DrawBMP(0,0,128,8,start_sta5);//开机外星人图标4	
		delay_ms(400);
		OLED_Clear();//关闭显示
		
}

/*功能菜单*/
void Display_menu(void)
{
	
	uint8_t date[64] = {0};
	uint8_t time[64] = {0};
	//获取日期
	RTC_GetDate(RTC_Format_BCD,&RTC_DateStructure);
	sprintf((char *)date,"20%02x/%02x/%02x",
			RTC_DateStructure.RTC_Year,
			RTC_DateStructure.RTC_Month,
			RTC_DateStructure.RTC_Date);
	
	
	OLED_ShowString(24,0,date,16);
	
	RTC_GetTime(RTC_Format_BCD,&RTC_TimeStructure);
	sprintf((char *)time,"%02x:%02x:%02x",
			RTC_TimeStructure.RTC_Hours,
			RTC_TimeStructure.RTC_Minutes,
			RTC_TimeStructure.RTC_Seconds);
	
	delay_ms(100);
	OLED_ShowString(32,2,time,16);
	
	OLED_ShowCHinese(16,4,104);	//按
	OLED_ShowCHinese(32,4,117);
	OLED_ShowCHinese(48,4,107);
	OLED_ShowCHinese(64,4,108);
	OLED_ShowCHinese(80,4,109);
	OLED_ShowCHinese(96,4,110);
	
	OLED_ShowCHinese(10,6,118);
	OLED_ShowCHinese(26,6,119);
	OLED_ShowCHinese(42,6,120);
	OLED_ShowCHinese(58,6,144);
	OLED_ShowCHinese(74,6,8);
	OLED_ShowCHinese(90,6,145);
	OLED_ShowString(106,6,"..",14);
	
}

/*功能选择界面*/
void Display_func(void)
{
	OLED_Clear();
	OLED_DrawBMP(0,0,50,7,lock);
	OLED_ShowChar(53,0,'1',12);	
	OLED_ShowCHinese(65,0,83);
	OLED_ShowCHinese(81,0,84);
	OLED_ShowCHinese(97,0,4);
	OLED_ShowCHinese(113,0,5);
	
	OLED_ShowChar(53,3,'2',12);
	OLED_ShowCHinese(65,3,20);
	OLED_ShowCHinese(81,3,21);
	OLED_ShowCHinese(97,3,4);
	OLED_ShowCHinese(113,3,5);
	OLED_ShowChar(53,6,'3',12);
	OLED_ShowCHinese(65,6,113);
	OLED_ShowCHinese(81,6,114);
	OLED_ShowCHinese(97,6,115);
	OLED_ShowCHinese(113,6,116);
	while(1)
	{
		/*变量声明*/
		int32_t rt;
		uint16_t id=1;
		uint32_t timeout=0;
		static char flag = 0;
		char num[16] = {0};
		int32_t card_flag = 0;
		int32_t j = 0;
		int32_t hand_rt = 0;
		
		
		flag = get_key_status();
		Gesture_test();
		sg_angle(0);
		switch(flag)
		{
			case '*':
			printf("返回上一级菜单\r\n");	
			break;
			/*手势识别*/
			case HAND_STATE_UNLOCK:
				printf("您已进入手势识别模式\r\n");
				OLED_Clear();
				OLED_DrawBMP(0,0,64,8,hand);
				OLED_ShowCHinese(60,2,135);
				OLED_ShowCHinese(76,2,136);
				OLED_ShowCHinese(92,2,137);
				OLED_ShowCHinese(108,2,138);

				while(hand_rt == 0)
				{
					hand_rt = Gesture_test();	
				}
					
				if(hand_rt == 2 || hand_rt == 3)
				{
					OLED_Clear();
					OLED_DrawBMP(0,0,64,8,hand);
					OLED_ShowCHinese(60,0,141);
					OLED_ShowCHinese(76,0,142);
					OLED_ShowCHinese(92,0,135);
					OLED_ShowCHinese(108,0,136);
					delay_xms(50);
					OLED_ShowCHinese(60,2,135);
					OLED_ShowCHinese(76,2,136);
					OLED_ShowCHinese(92,2,53);
					OLED_ShowCHinese(108,2,79);
					
					
					delay_xms(50);
					
					display();
					display();
					
					/*模拟开门*/
					sg_init();
					sg_angle(0);
					sg_angle(45);
					sg_angle(135);
					sg_angle(180);
					
					OLED_ShowCHinese(60,4,4);
					OLED_ShowCHinese(76,4,2);
					OLED_ShowCHinese(92,4,64);
					OLED_ShowCHinese(108,4,65);
					printf("手势识别开锁成功\r\n");
					search_unlock_record_admin(4,rand()%4);
				}else if(hand_rt < 0)
				{
					OLED_Clear();
					OLED_DrawBMP(0,0,64,8,hand);
					OLED_ShowCHinese(62,2,8);
					OLED_ShowCHinese(78,2,9);
					OLED_ShowCHinese(60,2,135);
					OLED_ShowCHinese(76,2,136);
					
					OLED_ShowString(63,4,"*",16);
					OLED_ShowCHinese(77,4,33);
					OLED_ShowCHinese(93,4,34);
				}
				
	
			break;
			
			/*指纹解锁*/
			case SFM_UNLOCK:
				OLED_Clear();
				OLED_DrawBMP(0,0,64,6,touch);
				OLED_ShowCHinese(62,2,26);
				OLED_ShowCHinese(78,2,113);
				OLED_ShowCHinese(94,2,8);
				OLED_ShowCHinese(110,2,9);
				
				printf("\r\n\r\n=====================================\r\n\r\n");
				printf("执行刷指纹操作,请将手指放到指纹模块触摸感应区\r\n");
				timeout=0;
			
				/* 显示蓝色 */
				sfm_ctrl_led(0x06,0x06,0x32);
				
				while((sfm_touch_check()!=SFM_ACK_SUCCESS) && (timeout<10))
				{
					timeout++;
				}
				
				if(timeout>=10)
				{
					printf("没有检测到手指，请重新操作!\r\n");
					
					/* 恢复光圈全亮->全灭，周期2秒 */
					sfm_ctrl_led(0x00,0x07,0xC8);
					
					OLED_Clear();
					OLED_DrawBMP(0,0,64,6,touch);
					OLED_ShowCHinese(62,2,8);
					OLED_ShowCHinese(78,2,9);
					OLED_ShowCHinese(94,2,120);
					OLED_ShowCHinese(110,2,121);
					OLED_ShowString(63,4,"*",16);
					OLED_ShowCHinese(77,4,33);
					OLED_ShowCHinese(93,4,34);
					PFout(8) = 1;delay_ms(500);PFout(8) = 0;
					continue;
				}
				
				printf("检测到手指，现在开始刷指纹...\r\n");
				
				rt=sfm_compare_users(&id);
				
				if(rt == SFM_ACK_SUCCESS)
				{
					/* 成功:光圈显示绿色 */
					sfm_ctrl_led(0x05,0x05,0x32);
					
					delay_ms(1000);
					
					/* 成功，蜂鸣器嘀一声示意 */
					PFout(8) = 1;delay_ms(50);PFout(8) = 0;						
				}
				else
				{
					/* 失败:光圈显示红色 */
					sfm_ctrl_led(0x03,0x03,0x32);
					
					OLED_Clear();
					OLED_DrawBMP(0,0,64,6,touch);
					OLED_ShowCHinese(62,2,8);
					OLED_ShowCHinese(78,2,9);
					OLED_ShowCHinese(94,2,120);
					OLED_ShowCHinese(110,2,121);
					OLED_ShowString(63,4,"*",16);
					OLED_ShowCHinese(77,4,33);
					OLED_ShowCHinese(93,4,34);
					PFout(8) = 1;delay_ms(500);PFout(8) = 0;
					
					/* 若id为0，则比对不成功的*/
					if(id == 0)
					{
						printf("指纹比对不成功\r\n");
					}
					
					/* 恢复光圈全亮->全灭，周期2秒 */
					sfm_ctrl_led(0x00,0x07,0xC8);
					
					delay_ms(1000);		
					continue;
				}
				
				/* 若id为0，则比对不成功的*/
				printf("刷指纹 %s 识别到id=%d\r\n",sfm_error_code(rt),id);
				
				/* 恢复光圈全亮->全灭，周期2秒 */
				sfm_ctrl_led(0x00,0x07,0xC8);
				
				/*识别成功*/
				display();
				
				/*模拟开门*/
				sg_init();
				sg_angle(0);
				sg_angle(45);
				sg_angle(135);
				sg_angle(180);
				
				search_unlock_record_admin(0,id-1);
				printf("指纹解锁成功\r\n");
				
				OLED_Clear();
				OLED_DrawBMP(0,0,64,6,touch);
				OLED_ShowCHinese(62,2,4);
				OLED_ShowCHinese(78,2,2);
				OLED_ShowCHinese(94,2,64);
				OLED_ShowCHinese(110,2,65);
				
				
			break;
			
			/*密码解锁*/
			case PASSWORD_UNLOCK:
				
				OLED_Clear();
				OLED_ShowCHinese(18,0,26);
				OLED_ShowCHinese(36,0,27);
				OLED_ShowCHinese(54,0,28);
				OLED_ShowCHinese(72,0,20);
				OLED_ShowCHinese(90,0,21);
				OLED_ShowString(18,3,"  -------- ",12);
				OLED_ShowString(0,6,"#:",12);
				OLED_ShowCHinese(14,6,29);
				OLED_ShowCHinese(28,6,30);
				OLED_ShowString(56,6,"C:",12);		
				OLED_ShowCHinese(70,6,33);		
				OLED_ShowCHinese(84,6,34);			
				
				for(j = 0;j < 16;j++)
				{
					key_state = 'n';
					while(key_state == 'n')
					{
						key_state = loop_get_key_sta();
					}
					if(key_state == 'C')	
					{
						OLED_Clear();
						OLED_ShowString(16,3,"*",12);
						OLED_ShowCHinese(32,3,29);
						OLED_ShowCHinese(48,3,30);
						OLED_ShowCHinese(64,3,33);
						OLED_ShowCHinese(80,3,34);
						goto end_of_loop;
					}
					if(key_state == '#')	{break;}
					switch(j)
					{
						case 0:
						OLED_ShowString(18,3," *------- ",12);
						break;
						case 1:
						OLED_ShowString(18,3," **------ ",12);
						break;
						case 2:
						OLED_ShowString(18,3," ***----- ",12);						
						break;
						case 3:
						OLED_ShowString(18,3," ****---- ",12);
						break;
						case 4:
						OLED_ShowString(18,3," *****--- ",12);
						break;
						case 5:
						OLED_ShowString(18,3," ******-- ",12);
						break;
						case 6:
						OLED_ShowString(18,3," *******- ",12);
						break;
						case 7:
						OLED_ShowString(18,3," ******** ",12);
						break;
					}
					num[j] = key_state;
					printf("%c\r\n",key_state);
				}num[j] = '\0';
				
				printf("您所输入的密码为 %s\r\n",num);
				/*密码正确处理*/
				if(strstr(password,num))
				{
					OLED_Clear();
					OLED_DrawBMP(0,0,64,7,house);
					OLED_ShowCHinese(65,2,27);
					OLED_ShowCHinese(81,2,28);
					OLED_ShowCHinese(97,2,53);
					OLED_ShowCHinese(113,2,79);
					
					display();
					sg_init();
					sg_angle(0);
					sg_angle(45);
					sg_angle(135);
					sg_angle(180);
					delay_xms(200);
					
					search_unlock_record_admin(1,rand()%4);
					OLED_ShowString(68,4,"*",16);
					OLED_ShowCHinese(84,4,33);
					OLED_ShowCHinese(100,4,34);
					
					
				}else{
					
					OLED_Clear();
					OLED_DrawBMP(0,0,64,7,house);
					OLED_ShowCHinese(65,2,20);
					OLED_ShowCHinese(81,2,21);
					OLED_ShowCHinese(97,2,35);
					OLED_ShowCHinese(113,2,36);
					display();
					OLED_ShowString(68,4,"*",16);
					OLED_ShowCHinese(84,4,33);
					OLED_ShowCHinese(100,4,34);
				}
				
				
			break;

			/*RFID解锁*/
			case RFID_UNLOCK:
				MFRC522_Initializtion();	
				OLED_Clear();
				delay_xms(10);
				OLED_DrawBMP(0,0,60,4,RFID);
				OLED_ShowCHinese(66,3,26);
				OLED_ShowCHinese(82,3,113);
				OLED_ShowCHinese(98,3,116);
				while(card_flag == 0)
				{
					card_flag = MFRC522Test();
				}
				if(card_flag == 1 || card_flag == 2)
				{
					sg_init();
					sg_angle(0);
					sg_angle(45);
					sg_angle(135);
					sg_angle(180);
					
					printf("刷卡成功\r\n");
					printf("正在保存记录\r\n");
					
					switch(card_flag)
					{
						case 1:search_unlock_record_admin(2,2);break;	//儿子
						case 2:search_unlock_record_admin(2,3);break;	//女儿
					}
					
					OLED_Clear();
					delay_xms(300);
					OLED_DrawBMP(0,0,60,4,RFID);
					OLED_ShowCHinese(64,1,113);
					OLED_ShowCHinese(80,1,116);
					OLED_ShowCHinese(96,1,24);
					OLED_ShowCHinese(112,1,25);
					OLED_ShowCHinese(68,3,14);
					OLED_ShowCHinese(84,3,4);
					OLED_ShowCHinese(100,3,2);
					display();
				
				}else{
					OLED_Clear();
					printf("刷卡失败\r\n");
					delay_xms(300);
					OLED_DrawBMP(0,0,60,4,RFID);
					OLED_ShowCHinese(64,1,113);
					OLED_ShowCHinese(80,1,116);
					OLED_ShowCHinese(96,1,120);
					OLED_ShowCHinese(112,1,121);
					OLED_ShowString(68,3,"*",16);
					OLED_ShowCHinese(84,3,33);
					OLED_ShowCHinese(100,3,34);
				
					display();
					display();
				
				}
			break;			
		}
	end_of_loop:
		if(flag == '*')	{OLED_Clear();break;}

	}
}


/*手势识别*/
int Gesture_test(void)
{
	uint8_t i;
    uint8_t status;

	uint8_t data[2]={0x00};
	uint16_t gesture_data;
	
	paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
	
	for(i=0;i<GESTURE_SIZE;i++)
	{
		GS_Write_Byte(gesture_arry[i][0],gesture_arry[i][1]);//手势识别模式初始化
	}
	
	paj7620u2_selectBank(BANK0);//切换回BANK0寄存器区域
	
	i=0;

	//读取手势状态			
	status = GS_Read_nByte(PAJ_GET_INT_FLAG1,2,&data[0]);
	
	if(!status)
	{   
		gesture_data =(uint16_t)data[1]<<8 | data[0];
		
		if(gesture_data) 
		{
			switch(gesture_data)
			{
				
				case GES_CLOCKWISE:             
							   printf("检测到手势顺时针\r\n");     return 1;
				case GES_COUNT_CLOCKWISE:   
							   printf("检测到手势逆时针\r\n"); return 2; //逆时针

				default:   return -1;
				
			}

			
		}
	}
	
	delay_ms(50);
	return 0;
}

void display_mqtt_unlock(void)
{
	PFout(9)=0;//控制灯亮
							
	OLED_Clear();
	OLED_DrawBMP(0,0,64,7,wifi);
	OLED_ShowCHinese(60,2,120);
	OLED_ShowCHinese(76,2,144);
	OLED_ShowCHinese(92,2,4);
	OLED_ShowCHinese(108,2,2);
	OLED_ShowCHinese(64,4,13 );
	
	display();
	display();
	
	sg_angle(0);
	sg_angle(45);
	sg_angle(135);
	sg_angle(180);
	
	delay_xms(40);
	OLED_Clear();
	OLED_DrawBMP(0,0,64,7,wifi);
	OLED_ShowCHinese(64,3,12);
	OLED_ShowCHinese(80,3,144);
	OLED_ShowCHinese(96,3,4);
	OLED_ShowCHinese(112,3,2);
	OLED_ShowCHinese(64,6,24);
	OLED_ShowCHinese(80,6,25);
	
	OLED_Clear();
	printf("无线解锁成功\r\n");
	search_unlock_record_admin(3,rand()%4);
}

/*看门狗标志判断*/
void WDG_RST_FLAG(void)
{
		if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET)      
		{													
			printf("IWDG reset cpu\r\n");					
		}													
		else if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET) 
		{													
			printf("WWDG reset cpu\r\n");					
		}													
		else												
		{													
			printf("normal reset cpu\r\n");					
		}													
}


/*串口数据转换实现*/
void usart_str_load(uint8_t *str_buf,uint32_t *usart_event,uint32_t *usart_cnt)
{
	static uint32_t i = 0;
	char *p;

	printf("str_buf = %s\r\n",str_buf);
	
	if(strstr((const char *)str_buf,"UNLOCK"))
	{
		OLED_Clear();
		OLED_DrawBMP(0,0,64,7,wifi);
		OLED_ShowCHinese(60,2,120);
		OLED_ShowCHinese(76,2,144);
		OLED_ShowCHinese(92,2,4);
		OLED_ShowCHinese(108,2,2);
		OLED_ShowCHinese(64,4,13 );
		
		display();
		display();
		
		sg_angle(0);
		sg_angle(45);
		sg_angle(135);
		sg_angle(180);
		
		delay_xms(40);
		OLED_Clear();
		OLED_DrawBMP(0,0,64,7,wifi);
		OLED_ShowCHinese(64,3,12);
		OLED_ShowCHinese(80,3,144);
		OLED_ShowCHinese(96,3,4);
		OLED_ShowCHinese(112,3,2);
		OLED_ShowCHinese(64,6,24);
		OLED_ShowCHinese(80,6,25);
		
		OLED_Clear();
		printf("无线解锁成功\r\n");
		search_unlock_record_admin(rand()%4,3);
		
	}
	
	if(strstr((const char *)str_buf,"RECORD_PRINT"))
	{
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
	}
	
	
	if(strstr((const char *)str_buf,"DATE SET"))
	{
		//以等号分割字符串\r\n
		strtok((char *)str_buf,"-");

		//获取年
		p=strtok(NULL,"-");
		//2017-2000=17 
		i = atoi(p)-2000;
		//转换为16进制 17 ->0x17
		i = (i/10)*16+i%10;
		RTC_DateStructure.RTC_Year = i;
		

		//获取月
		p=strtok(NULL,"-");
		i= atoi(p);
		//转换为16进制
		i= (i/10)*16+i%10;						
		RTC_DateStructure.RTC_Month=i;

		p=strtok(NULL,"-");
		i=atoi(p);
		//转换为16进制
		i= (i/10)*16+i%10;		
		RTC_DateStructure.RTC_Date = i;
		printf("%x\r\n",i);
		//获取星期
		p=strtok(NULL,"-");
		i=atoi(p);
		//转换为16进制
		i= (i/10)*16+i%10;						
		RTC_DateStructure.RTC_WeekDay = i;
		printf("%x\r\n",i);
		//设置日期
		RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
		printf("日期修改成功\r\n");
		
		printf("当前日期为 %x/%x/%x \r\n",RTC_DateStructure.RTC_Year,
										 RTC_DateStructure.RTC_Month,
										 RTC_DateStructure.RTC_Date);
	}													

	/* 设置时间 */
	if(strstr((const char *)str_buf,"TIME SET"))
	{
		//以等号分割字符串
		strtok((char *)str_buf,"-");
		 
		//获取时
		p=strtok(NULL,"-");
		i = atoi(p);
		
		//通过时，判断是AM还是PM
		if(i<12)
			RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
		else
			RTC_TimeStructure.RTC_H12     = RTC_H12_PM;
			
		//转换为16进制
		i= (i/10)*16+i%10;
		RTC_TimeStructure.RTC_Hours   = i;
		
		//获取分
		p=strtok(NULL,"-");
		i = atoi(p);						
		//转换为16进制
		i= (i/10)*16+i%10;	
		RTC_TimeStructure.RTC_Minutes = i;
		
		//获取秒
		p=strtok(NULL,"-");
		i = atoi(p);						
		//转换为16进制
		i= (i/10)*16+i%10;					
		RTC_TimeStructure.RTC_Seconds = i; 					
		
		RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure); 
		
		printf("时间修改成功\r\n");
		
		printf("当前时间为 %x/%x/%x \r\n",RTC_TimeStructure.RTC_Hours,
										 RTC_TimeStructure.RTC_Minutes,
										 RTC_TimeStructure.RTC_Seconds);
	}
	
	/*温湿度数据采集*/
	if(strstr((const char *)str_buf,"start"))
	{
		/*开始温湿度数据采集*/
		g_dht11_start = 1;
		
		printf("开启温湿度的数据采集\r\n");
	}

	/*判断接收到的字符串为停止*/
	if(strstr((const char *)str_buf,"stop") )
	{
		//停止温湿度数据采集
		g_dht11_start = 0;

		printf("停止温湿度的数据采集\r\n");
	}

	/*清空*/
	if(strstr((const char *)str_buf,"CLEAR_RECORD") )
	{
		//清空所有记录
		printf("清空所有记录......\r\n");

		//扇区擦除
		flash_erase_record();

		printf("所有记录均已删除\r\n");

		//清零记录计数值
		dht11_rec_cnt = 0;

	}

	
	//清空串口1数据计数值
	*usart_cnt = 0;			
	
	//清空串口1接收数据事件
	*usart_event = 0;
	
	//清空串口1接收数据缓冲区
	memset((char *)str_buf,0,50);
	
}


void flash_read_record(char *pbuf,uint32_t record_count)
{
	uint32_t addr_start=ADDR_FLASH_SECTOR_4+record_count*64;
	uint32_t addr_end  =addr_start+64;

	uint32_t i=0;
	
	while (addr_start < addr_end)
	{
		*(uint32_t *)&pbuf[i] = *(__IO uint32_t*)addr_start;

		addr_start+=4;
		
		i = i + 4;
	}

}

void flash_erase_record(void)
{
	
	/* 如果不擦除，写入会失败，读取的数据都为0 */
	printf("FLASH_EraseSector start\r\n");
	
	if (FLASH_EraseSector(FLASH_Sector_4, VoltageRange_3) != FLASH_COMPLETE)
	{ 
			printf("Erase error\r\n");
			return;
	}

	printf("FLASH_EraseSector ends\r\n");

}

uint32_t flash_write_record(char *pbuf,uint32_t record_count)
{
	uint32_t addr_start=ADDR_FLASH_SECTOR_4+record_count*64;
	uint32_t addr_end  =addr_start+64;

	uint32_t i=0;
	
	while (addr_start < addr_end)
	{
		//每次写入数据是4个字节
		if (FLASH_ProgramWord(addr_start, *(uint32_t *)&pbuf[i]) == FLASH_COMPLETE)
		{
			//地址每次偏移4个字节
			addr_start +=4;
			
			i+=4;
		}

		else
		{ 
			printf("flash write record fail,now goto erase sector!\r\n");
			
			//重新擦除扇区
			flash_erase_record();

			return 1;
		}
	}
	return 0;
}


