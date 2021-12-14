#ifndef __INIT_USART_H__
#define __INIT_USART_H__

#include <stdio.h>

extern volatile uint8_t  g_usart1_rx_buf[512];
extern volatile uint32_t g_usart1_rx_cnt;
extern volatile uint32_t g_usart1_rx_end;

extern volatile uint8_t  g_usart2_rx_buf[512];
extern volatile uint32_t g_usart2_rx_cnt;
extern volatile uint32_t g_usart2_rx_end;

extern volatile uint8_t  g_usart3_rx_buf[512];
extern volatile uint32_t g_usart3_rx_cnt;
extern volatile uint32_t g_usart3_rx_end;

extern void usart1_init(uint32_t baud);
extern void usart2_init(uint32_t baud);
extern void usart3_init(uint32_t baud);

extern void usart_send_str(USART_TypeDef* USARTx,char *str);
extern void usart_send_bytes(USART_TypeDef* USARTx,uint8_t *buf,uint32_t len);



#endif
