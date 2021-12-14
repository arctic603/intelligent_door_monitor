#ifndef __INCLUDES_H__
#define __INCLUDES_H__

/*标准C库*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


/*位带操作*/
#include "sys.h"

/*主板硬件相关*/
#include "init_led.h"
#include "key.h"
#include "EXTI.h"
#include "beep.h"
#include "init_USART.h"
#include "delay.h"			
#include "Tim_init.h"		
#include "WWDG.h"
#include "RTC.h"
#include "ADC.h"
#include "SPI.h"
#include "I2C.h"
#include "flash.h"

/*外设相关功能*/
#include "DHT11.h"		//温湿度
#include "OLED.h"		//OLED
#include "PAJ7620.h"
#include "MFRC522.h"	//RFID
#include "sg90.h"		//舵机
#include "sfm.h"		//指纹模块


/*串口1的相关变量*/
static uint8_t  g_usart1_buf[64] = {0};	//串口1的缓冲区
static uint32_t g_usart1_cnt 	  		  = 0;		//串口1统计传输的字符个数
static uint32_t g_usart1_event            = 0;		//串口1统计事件	

/*RTC时钟变量*/
static volatile uint32_t g_rtc_wakeup_event  = 0;	//RTC时钟唤醒事件标志
static volatile uint32_t g_rtc_alarm_a_event = 0;	//RTC闹钟A唤醒事件标志

/*温度记录事件*/
static volatile uint32_t DHT11_buf[4] = {0}; 
static volatile uint32_t g_tim_DHT11_event = 0;
static volatile uint32_t g_dht11_start=0;

/*蓝牙相关变量*/
static uint32_t g_ble_event = 0;					//蓝牙统计事件
static uint32_t g_ble_cnt   = 0;					//蓝牙统计传输的字符个数
static uint8_t g_ble_buf[50] = {0};		//蓝牙缓冲区

static uint8_t 	dht11_data[4];
static uint32_t	dht11_rec_cnt=0;

static uint32_t key_sta = 0;						//管理员模式识别
static uint32_t key_sta_back = 0;
static char key_state = 'n';
/*初始化结构体*/
static RTC_DateTypeDef  	 RTC_DateStructure;
static RTC_TimeTypeDef  	 RTC_TimeStructure;
static GPIO_InitTypeDef 	 GPIO_InitStructure;
static EXTI_InitTypeDef 	 EXTI_InitStructure;
static ADC_InitTypeDef       ADC_InitStructure;
static ADC_CommonInitTypeDef ADC_CommonInitStructure;
static NVIC_InitTypeDef 	 NVIC_InitStructure;

/*闪存相关*/
static uint32_t uwStartSector 	= 0;
static uint32_t uwEndSector 	= 0;
static uint32_t uwSectorCounter = 0;

/*管理员模式*/
static uint32_t EXTI_flag = 0;
static char unlock_record_npc[][10] = {{"爸爸"},{"妈妈"},{"儿子"},{"女儿"}};
static char lock_func_chose[][15] = {{"指纹"},{"密码"},{"门禁卡"},{"蓝牙"},{"手势"}}; 

/*RFID 模块相关变量*/
#if 0
//数字的ASCII码
uc8 numberascii[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
//显示缓冲区
u8  dispnumber5buf[6];
u8  dispnumber3buf[4];
u8  dispnumber2buf[3];
//MFRC522数据区
u8  mfrc552pidbuf[18];
u8  card_pydebuf[2];
u8  card_numberbuf[5];
u8  card_key0Abuf[6]={0xff,0xff,0xff,0xff,0xff,0xff};
u8  card_writebuf[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
u8  card_readbuf[18];
//SM05-S数据区
u8  sm05cmdbuf[15]={14,128,0,22,5,0,0,0,4,1,157,16,0,0,21};

#endif


/*临时变量*/
static uint32_t	i=0;
static char buf[128]={0};

/*相关功能函数*/
void INIT_FUNC(void);
void WDG_RST_FLAG(void);
void Display_start(void);
void Display_menu(void);
void Display_func(void);
void usart_str_load(uint8_t *,uint32_t *,uint32_t *);


/*RFID部分SPI相关函数*/
void flash_read_record(char *pbuf,uint32_t record_count);
void flash_erase_record(void);
uint32_t flash_write_record(char *pbuf,uint32_t record_count);

/*管理员模式下的相关*/
void add_sfm_admin(void);
void get_sfm_admin(void);
void del_sfm_admin(void);
int search_unlock_record_admin(int , int );

/*手势识别*/
int Gesture_test(void);

void display(void);

#endif

