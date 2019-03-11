#ifndef __TSENSOR_H
#define __TSENSOR_H	
#include "stm32f10x.h"

 		 
//#define ADC_CH_TEMP  	 //温度传感器通道
   
u16  T_Get_Temp(void);  //取得温度值
void T_Adc_Init(void); //ADC通道初始化
u16  T_Get_Adc(u8 ch); //获得某个通道值  
u16  T_Get_Adc_Average(u8 ch,u8 times);//得到某个通道10次采样的平均值 	

u16 GetGasAdc_Average(u8 ch,u8 times);
u16 GetGasAdc(u8 ch);
void CO_ADC_Init(void);  //ADC通道初始化;

void LightADC_Init(void);
u16 GetADC2_Val(u8 ch);
u16 GetADC2_Average(u8 ch,u8 times);


u16 GetADC_Average(u8 ch,u8 times);
void LightADC_Init(void);

u16 GetCOAdC_Average(u8 ch,u8 times);
void SP_ADC_Init(void);  //ADC通道初始化

u16 GetSpAdC_Average(u8 ch,u8 times);
	u16 GetADC_Average(u8 ch,u8 times);
u16 GetGasAdc(u8 ch)   ;

#endif 


