#ifndef __LED_H
#define __LED_H

#include "sys.h"

#define LED1 PCout(13)// PC13


extern void Init_LEDpin(void);


#define LED0_ON         GPIO_ResetBits(GPIOC,GPIO_Pin_13)
#define LED0_OFF        GPIO_SetBits(GPIOC,GPIO_Pin_13)

#define LED1_ON         GPIO_ResetBits(GPIOC,GPIO_Pin_14)
#define LED1_OFF        GPIO_SetBits(GPIOC,GPIO_Pin_14)

#define LED2_ON       GPIO_ResetBits(GPIOC,GPIO_Pin_15)  
#define LED2_OFF      GPIO_SetBits(GPIOC,GPIO_Pin_15) 

 



void PortInit(void);  //所有端口初始化;

#endif

