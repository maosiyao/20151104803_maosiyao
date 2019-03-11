#ifndef __12864_H
#define __12864_H

#include "sys.h"






extern u8 LCD_Buf[];
extern u8 LCD_RefFlag;

#define SENDCOMM 0XF8     
#define SENDDATA 0XFA      


void LCD_Init(void);  //LCD��ʾ��ʼ����Ҫ����һ��
void LCD_DispChar(unsigned char add,unsigned char c);   //�ַ���ʾ
void LCD_DispStr(unsigned char add,unsigned char *str); //�ַ�����ʾ
void LCD_Clr(void);   //��ʾ���� 
void LCD_Ref(void);   //��ʾˢ��

void TempDisplay(unsigned char add,unsigned char temp);
void HumiDisplay(unsigned char add,unsigned char humi);

void DisplayU16(unsigned char add,unsigned int dat);

#endif


















