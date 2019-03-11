#ifndef __KEY4_H
#define __KEY4_H

#include "sys.h"

#define KEY_IDLE      0
#define KEY_ENSURE 	  1
#define KEY_PRESS     2
#define KEY_WAIT_UP   3


void KEY_PortInit(void);

extern  unsigned char KeyVal;
extern  unsigned char KeyTime;
void KEY_Scan(void);	

#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)
#define KEY2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)
#define KEY3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
#define KEY4  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)

extern unsigned char DOUBLE_EN;   //
 
#endif




