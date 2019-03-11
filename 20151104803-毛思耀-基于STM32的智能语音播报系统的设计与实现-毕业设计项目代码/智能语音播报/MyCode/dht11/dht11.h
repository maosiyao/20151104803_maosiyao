#ifndef __DHT11_H__
#define __DHT11_H__

#include "stm32f10x.h"


u8 DHT11_Check(void);
u8 DHT11_ReadData(u8 *temp,u8 *humi);


#endif







