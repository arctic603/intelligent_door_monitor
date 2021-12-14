#include "RTC.h"

static NVIC_InitTypeDef  NVIC_InitStructure;
static EXTI_InitTypeDef  EXTI_InitStructure;
static RTC_DateTypeDef  RTC_DateStructure;
static RTC_TimeTypeDef  RTC_TimeStructure;
static RTC_InitTypeDef  RTC_InitStructure;
static RTC_AlarmTypeDef RTC_AlarmStructure;

void RTC_CONFIG(void)
{
	/*Enable the PWR clock*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	
	/*Allow access to RTC*/
	PWR_BackupAccessCmd(ENABLE);
	
	#if 0
	/*使能低速外部时钟LSE*/
	RCC_LSEConfig(RCC_LSE_ON);
	
	/*检查该LSE是否有效*/
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	
	/*选择LSE作为RTC的硬件时钟源*/
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	#else
	/*若LSE无法工作时，启用LSI*/
	
	/*使能低速内部时钟LSI*/
	RCC_LSICmd(ENABLE);
	
	/*检查该LSI是否有效*/
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	
	/*选择LSI作为RTC的硬件时钟源*/
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	
	#endif
	
	/*使能RTC时钟*/
	RCC_RTCCLKCmd(ENABLE);
	
	/*等待RTC相关寄存器就绪*/
	RTC_WaitForSynchro();
	
	#if 0  //LSE
	
	/*Configure the RTC data Register and RTC prescaler*/
	RTC_InitStructure.RTC_AsynchPrediv = 127;					//异步分频系数
	RTC_InitStructure.RTC_SynchPrediv = 255;				//同步分频系数
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;	//24小时格式
	RTC_Init(&RTC_InitStructure);
	#else
	RTC_InitStructure.RTC_AsynchPrediv = 127;				//异步分频系数
	RTC_InitStructure.RTC_SynchPrediv = 249;				//同步分频系数
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;	//24小时格式
	RTC_Init(&RTC_InitStructure);
	
	#endif
	
	/*Set the date*/
	RTC_DateStructure.RTC_Year = 0x21;
	RTC_DateStructure.RTC_Month = RTC_Month_August;
	RTC_DateStructure.RTC_Date = 0x01;
	RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Sunday;
	RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
	
	/*Set the time*/
	RTC_TimeStructure.RTC_H12     = RTC_H12_PM;
	RTC_TimeStructure.RTC_Hours   = 0x19;
	RTC_TimeStructure.RTC_Minutes = 0x07;
	RTC_TimeStructure.RTC_Seconds = 0x55; 
	RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure); 
	
	//关闭唤醒功能
	RTC_WakeUpCmd(DISABLE);
	
	//为唤醒功能选择RTC配置好的时钟源
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
	
	//设置唤醒计数值为自动重载，写入值默认是0
	RTC_SetWakeUpCounter(1-1);
	
	//清除RTC唤醒中断标志
	RTC_ClearITPendingBit(RTC_IT_WUT);
	
	//使能RTC唤醒中断
	RTC_ITConfig(RTC_IT_WUT,ENABLE);
	
	//使能唤醒功能
	RTC_WakeUpCmd(ENABLE);
	
	/*Configure EXTI LINE22 配置外部中断控制线22*/
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;			//当前使用外部中断控制线22
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;		//上升沿触发中断 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//使能外部中断控制线22
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;			//允许RTC唤醒中断触发
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//抢占优先级为0x3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		//响应优先级为0x3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能
	NVIC_Init(&NVIC_InitStructure);
}

void RTC_GET_TIME(void)
{
	//获取时间
	RTC_GetTime(RTC_Format_BCD,&RTC_TimeStructure);
	printf("%02x:%02x:%02x\r\n",RTC_TimeStructure.RTC_Hours,RTC_TimeStructure.RTC_Minutes,RTC_TimeStructure.RTC_Seconds);

	//获取日期
	RTC_GetDate(RTC_Format_BCD,&RTC_DateStructure);
	printf("20%02x/%02x/%02xWeek:%x\r\n",RTC_DateStructure.RTC_Year,RTC_DateStructure.RTC_Month,RTC_DateStructure.RTC_Date,RTC_DateStructure.RTC_WeekDay);	

}

void RTC_RST_CONFIG(void)
{
		/* 若开机读取备份寄存器的值不等于0x1688，则进行rtc日期和时间的重置 */
		if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x8888)
		{
			
			RTC_CONFIG();
			/* 对备份寄存器0写入数据0x1688，用于标记已经初始化过 */
			RTC_WriteBackupRegister(RTC_BKP_DR0,0x8888);
		}
		else
		{
		
			/* Enable the PWR clock ，使能电源控制时钟*/
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
			
			/* Allow access to RTC，允许访问备份电路 */
			PWR_BackupAccessCmd(ENABLE);


			#if 0
			/* 使能LSE*/
			RCC_LSEConfig(RCC_LSE_ON);
			
			/* 检查该LSE是否有效*/  
			while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

			/* 选择LSE作为RTC的硬件时钟源*/
			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
			

			#else  
			//若LSE无法工作，可用内部LSI
			/* 使能LSI*/
			RCC_LSICmd(ENABLE);
			
			/* 检查该LSI是否有效*/  
			while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

			/* 选择LSI作为RTC的硬件时钟源*/
			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

			#endif

			/* Enable the RTC Clock，使能RTC时钟 */
			RCC_RTCCLKCmd(ENABLE);
			
			/* Wait for RTC APB registers synchronisation，等待RTC相关寄存器就绪 */
			RTC_WaitForSynchro();
			
			#if 0 //LSE
			/* Configure the RTC data register and RTC prescaler，配置RTC数据寄存器与RTC的分频值 */
			RTC_InitStructure.RTC_AsynchPrediv = 127;				//异步分频系数
			RTC_InitStructure.RTC_SynchPrediv = 255;				//同步分频系数
			RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;	//24小时格式
			RTC_Init(&RTC_InitStructure);
			#else //LSI
			/* Configure the RTC data register and RTC prescaler，配置RTC数据寄存器与RTC的分频值 */
			RTC_InitStructure.RTC_AsynchPrediv = 127;				//异步分频系数
			RTC_InitStructure.RTC_SynchPrediv = 249;				//同步分频系数
			RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;	//24小时格式
			RTC_Init(&RTC_InitStructure);

			#endif
			//关闭唤醒功能
			RTC_WakeUpCmd(DISABLE);
			
			//为唤醒功能选择RTC配置好的时钟源
			RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
			
			//设置唤醒计数值为自动重载，写入值默认是0
			RTC_SetWakeUpCounter(1-1);
			
			//清除RTC唤醒中断标志
			RTC_ClearITPendingBit(RTC_IT_WUT);
			
			//使能RTC唤醒中断
			RTC_ITConfig(RTC_IT_WUT, ENABLE);

			//使能唤醒功能
			RTC_WakeUpCmd(ENABLE);

			/* Configure EXTI Line22，配置外部中断控制线22 */
			EXTI_InitStructure.EXTI_Line = EXTI_Line22;			//当前使用外部中断控制线22
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//中断模式
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;		//上升沿触发中断 
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;			//使能外部中断控制线22
			EXTI_Init(&EXTI_InitStructure);
			
			NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;				//允许RTC唤醒中断触发
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	//抢占优先级为0x3
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;			//响应优先级为0x3
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能
			NVIC_Init(&NVIC_InitStructure);	
		}
}

void RTC_ALARM_A_INIT(void)
{
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);
	
	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_PM;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours 	 = 0x19;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0x09;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0X00;
	
	#if 0
	//RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
	//RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;	//屏蔽日期和星期，就是闹钟每天都生效
	#endif
	
	#if 0
	//指定1号生效
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel_Date = 0x01;
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel 		= RTC_AlarmDateWeekDaySel_Date;	//按照指定日期生效
	RTC_AlarmStructure.RTC_AlarmMask 				= RTC_AlarmMask_None;
	#endif
	
	#if 1
	//指定星期日生效
	RTC_AlarmStructure.RTC_AlarmDateWeekDay 	= RTC_Weekday_Sunday;
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel 	= RTC_AlarmDateWeekDaySel_WeekDay;//指定星期生效
	RTC_AlarmStructure.RTC_AlarmMask 			= RTC_AlarmMask_None;	//不屏蔽日期和星期，开放日期和星期的指定
	#endif
	
	RTC_SetAlarm(RTC_Format_BCD,RTC_Alarm_A,&RTC_AlarmStructure);
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
	
	/*Enable RTC Alarm A Interrupt*/
	/*使能闹钟A中断*/
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);
	RTC_ClearFlag(RTC_FLAG_ALRAF);
	
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStructure.EXTI_Line 	= EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode 	= EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* Enable the RTC Alarm Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel 					 = RTC_Alarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd  			     = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}




