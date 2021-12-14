#ifndef __FPUTC_H__
#define __FPUTC_H__

#include <stm32f4xx.h>
#include <stdio.h>


/*配置printf函数的调用前置*/
struct __FILE { int handle;  };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) 
{	
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return(ch);
}


#endif
