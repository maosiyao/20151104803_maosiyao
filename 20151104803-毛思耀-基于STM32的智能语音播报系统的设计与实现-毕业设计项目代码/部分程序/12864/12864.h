#ifndef __12864_H
#define __12864_H

#include "sys.h"


//CS   PB3
//SID  PB4
//CLK  PB5
#define LCD_CLK_SET    GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define LCD_CLK_RESET  GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define LCD_SID_SET    GPIO_SetBits(GPIOB,GPIO_Pin_4)
#define LCD_SID_RESET  GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define LCD_CS_SET     GPIO_SetBits(GPIOB,GPIO_Pin_3)
#define LCD_CS_RESET   GPIO_ResetBits(GPIOB,GPIO_Pin_3)




extern u8 LCD_Buf[];
extern u8 SparkF;    //????
extern u8 SparkA;    //??????
extern u8 SparkL;    //??????1
extern u8 LCD_RefFlag;

#define SENDCOMM 0XF8     //???????
#define SENDDATA 0XFA     //???????

void LCD_Init(void);
void Display_String(u8 x,u8 y,u8 *string);
void LCD_Clr(void);
void LCD_Task(void);  //显示刷新任务 20-50ms执行一次
void Display_u8(u8 x,u8 y,u16 num);
void Display_Char(u8 x,u8 y, u8 Char);
void LCD_LineClr(u8 line);
void DisplayOneChar(u8 add,u8 ch);
void DisplayU16(unsigned char x,unsigned char y,unsigned int dat);

void DHT_HumiDisplay(unsigned char add,unsigned char humi);
void TempDisplay(unsigned char add,float Temp);
#endif


















