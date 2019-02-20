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


#define IN   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)
#define OUT  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)
#define FIRE  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#define TOUCH  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)


#endif




