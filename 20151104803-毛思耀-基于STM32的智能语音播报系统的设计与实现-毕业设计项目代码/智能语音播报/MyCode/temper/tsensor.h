#ifndef __TSENSOR_H
#define __TSENSOR_H	
#include "stm32f10x.h"

 		 
//#define ADC_CH_TEMP  	 //�¶ȴ�����ͨ��
   
u16  T_Get_Temp(void);  //ȡ���¶�ֵ
void T_Adc_Init(void); //ADCͨ����ʼ��
u16  T_Get_Adc(u8 ch); //���ĳ��ͨ��ֵ  
u16  T_Get_Adc_Average(u8 ch,u8 times);//�õ�ĳ��ͨ��10�β�����ƽ��ֵ 	

u16 GetGasAdc_Average(u8 ch,u8 times);
u16 GetGasAdc(u8 ch);
void CO_ADC_Init(void);  //ADCͨ����ʼ��;

void LightADC_Init(void);
u16 GetADC2_Val(u8 ch);
u16 GetADC2_Average(u8 ch,u8 times);


u16 GetADC_Average(u8 ch,u8 times);
void LightADC_Init(void);

u16 GetCOAdC_Average(u8 ch,u8 times);
void SP_ADC_Init(void);  //ADCͨ����ʼ��

u16 GetSpAdC_Average(u8 ch,u8 times);
	u16 GetADC_Average(u8 ch,u8 times);
u16 GetGasAdc(u8 ch)   ;

#endif 


