#ifndef __RTC_H__
#define __RTC_H__

#include <stm32f4xx.h>
#include <stdio.h>

void RTC_CONFIG(void);
void RTC_GET_TIME(void);
void RTC_RST_CONFIG(void);
void RTC_ALARM_A_INIT(void);


#if 0
void RTC_Alarm_IRQHandler(void)
{
	
	if(RTC_GetITStatus(RTC_IT_ALRA) == SET)
	{
		printf("RTC_Alarm_IRQHandler\r\n");
		g_rtc_alarm_a_event = 1;
		RTC_ClearITPendingBit(RTC_IT_ALRA);
		EXTI_ClearITPendingBit(EXTI_Line17);
	}
}
#endif



#endif
