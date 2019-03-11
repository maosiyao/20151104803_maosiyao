#ifndef __12864_H
#define __12864_H

#include "sys.h"






extern u8 LCD_Buf[];
extern u8 LCD_RefFlag;

#define SENDCOMM 0XF8     
#define SENDDATA 0XFA      


void LCD_Init(void);  //LCD显示初始化需要调用一次
void LCD_DispChar(unsigned char add,unsigned char c);   //字符显示
void LCD_DispStr(unsigned char add,unsigned char *str); //字符串显示
void LCD_Clr(void);   //显示清屏 
void LCD_Ref(void);   //显示刷新

void TempDisplay(unsigned char add,unsigned char temp);
void HumiDisplay(unsigned char add,unsigned char humi);

void DisplayU16(unsigned char add,unsigned int dat);

#endif


















