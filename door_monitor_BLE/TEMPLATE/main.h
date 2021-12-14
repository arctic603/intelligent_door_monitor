#ifndef __MAIN_H
#define __MAIN_H
 

#include "stm32f4xx.h"

#if defined (USE_STM324xG_EVAL)
#include "stm324xg_eval.h"

#elif defined (USE_STM324x7I_EVAL) 
#include "stm324x7i_eval.h"

#elif defined (USE_STM324x9I_EVAL) 
#include "stm324x9i_eval.h"

#else
 #error "Please select first the Evaluation board used in your application (in Project Options)"
#endif


   #if defined (USE_STM324xG_EVAL)
   #define LED1_PIN                         GPIO_Pin_6
   #define LED2_PIN                         GPIO_Pin_8



   #if defined (USE_STM324x7I_EVAL)    
   #define LED1_PIN                         GPIO_Pin_6
   #define LED2_PIN                         GPIO_Pin_8

 #endif /* USE_STM324x7I_EVAL */
 
	#if defined (USE_STM324x9I_EVAL)    
	#define LED1_PIN                         GPIO_Pin_6
	#define LED2_PIN                         GPIO_Pin_7

 #endif /* USE_STM324x9I_EVAL */

 #endif /* __MAIN_H */

