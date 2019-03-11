#include "stm32f10x.h"
#include <stdio.h>
#include "stm32f10x_adc.h"
#include "delay.h"
#include "usart.h"
#include "tsensor.h"
#include "led.h"
#include "timer.h"     
#include "string.h"
#include "key4.h"
#include "dht11.h"
#include "12864.h"
#include "xfs5152.h"
 

unsigned char INFO_1[]="ǰ����վ��ҩ��һ���³��ĳ˿����������ߣ��ϳ��ĳ˿���ˢ������Ϊ����Ҫ�ĳ˿�������лл";
unsigned char INFO_2[]="ǰ����վ��ҩ�Ӷ����³��ĳ˿����������ߣ��ϳ��ĳ˿���ˢ������Ϊ����Ҫ�ĳ˿�������лл";
unsigned char INFO_3[]="ǰ����վ��ҩ�������³��ĳ˿����������ߣ��ϳ��ĳ˿���ˢ������Ϊ����Ҫ�ĳ˿�������лл";
unsigned char INFO_4[]="ǰ����վ��ҩ���ģ��³��ĳ˿����������ߣ��ϳ��ĳ˿���ˢ������Ϊ����Ҫ�ĳ˿�������лл";
unsigned char INFO_5[]="ǰ����վ��ҩ���壬�³��ĳ˿����������ߣ��ϳ��ĳ˿���ˢ������Ϊ����Ҫ�ĳ˿�������лл";

unsigned char Flag=0;

unsigned char Temp=0;  //�¶�
unsigned char Humi=0;  //ʪ��
unsigned int Time=60;  //ʱ����
unsigned int CountTime=0;  //����ʱ


void DHT11_Init(void);
void PageDisInit(void);
void GetTempHumi(void);  //��ȡ��ʪ�Ⱥ��� 
void ReleaseJTAG_Pin(void);  //�ͷ���JTAG������ص����� 
void PageDisplay(void);
void GetTempHumi(void);
void ReceiveCheck(void);   //���ڽ��ղ�����
void SetInit(void); //��ʪ�Ȳ���ʱ���趨
void PageDisInit(void);

unsigned char PlayBuf[50]={0};

unsigned char Sta=0;
unsigned char Num=0;



void TempHumiPlay(void);
void CountDown(void);


//void XFS_Suspend(void);//��ͣ�ϳ�
//void XFS_Recover(void);//�����ϳ�
//void XFS_Stop(void);  //ֹͣ�ϳ�

void HandPlay(void)
{
	if(KeyVal=='1')
	{
		
		KeyVal=0;
		LED0_ON;
		Num++;
		if(Num>5)
			Num=1;
		if(Num==1)
			XFS_Start(INFO_1);
		else if(Num==2)
			XFS_Start(INFO_2);
		else if(Num==3)
			XFS_Start(INFO_3);
		else if(Num==4)
			XFS_Start(INFO_4);
		else if(Num==5)
			XFS_Start(INFO_5);		
		
		delay_ms(500); 
		LED0_OFF;
	}
	
	if(KeyVal=='2')
	{
		KeyVal=0;
		XFS_Suspend();
	}
	if(KeyVal=='3')
	{
		KeyVal=0;
		XFS_Recover();
	}
	
	if(KeyVal=='E')
	{
		KeyVal=0;
		LED0_ON;
		if(Num==1)
			XFS_Start(INFO_1);
		else if(Num==2)
			XFS_Start(INFO_2);
		else if(Num==3)
			XFS_Start(INFO_3);
		else if(Num==4)
			XFS_Start(INFO_4);
		else if(Num==5)
			XFS_Start(INFO_5);		
		
		delay_ms(500); 
		LED0_OFF;
	}	
	
}




int main(void)
{		
	delay_init();
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�//
	ReleaseJTAG_Pin();
	LCD_Init();
	PortInit();
	KEY_PortInit();
	TIM3_Int_Init(200,7199);  //20MS��ʱ��
	SetInit();
	PageDisInit();
	uart_init(9600); 
	USART2_Init(9600);
	delay_ms(500);
	XFS_Start((unsigned char*)"        ");
	XFS_Start((unsigned char*)"        ");
	CountTime = Time;
	while(1)
	{
		GetTempHumi();
		PageDisplay();
		HandPlay();
		TempHumiPlay();
		LCD_Ref();
		ReceiveCheck();
		delay_ms(50);    //��ʱ
	}
}



void TempHumiPlay(void)
{
	if(CountTime==0)
	{
		LED1_ON;
		sprintf((char*)PlayBuf,"��ǰ�¶�:%d���ö�  ��ǰʪ��%d%%RH\0",Temp,Humi);
		PageDisplay();
		LCD_Ref();
		LCD_Ref();
		XFS_Start(PlayBuf);
		delay_ms(500);
		delay_ms(500);
		delay_ms(500);
		delay_ms(500);
		delay_ms(500);
		delay_ms(500);
		CountTime = Time;
		LED1_OFF;
	}
}

void CountDown(void)
{
	static unsigned char T20MS=0;
	
	T20MS++;
	if(T20MS>=50)
	{
		T20MS=0;
		if(CountTime>0)
			CountTime--;
	}
}



void ReleaseJTAG_Pin(void)  //�ͷ���JTAG������ص�����
{
  GPIO_InitTypeDef   GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA 
												| RCC_APB2Periph_GPIOB 
												|	RCC_APB2Periph_GPIOC 
												| RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}



void ReceiveCheck(void)   //   
{
	if(U1_Continue>0)
	{
		LED2_ON;
		U1_Continue++;
		if(U1_Continue>=10)
		{
			U1_Buf[U1_Con]='\0';
			U1_Continue=0;
			U1_Con=0;
			XFS_Start(U1_Buf);
			LED2_OFF;
		}
	}
}

void SetInit(void) //��ʪ�Ȳ���ʱ���趨
{
	LCD_Clr();	
	LCD_DispStr( 0,(unsigned char*)"��ʪ���Զ�����");
	LCD_DispStr(16,(unsigned char*)"ʱ�����趨:");
	while(1)
	{
		DisplayU16(32,Time);
		if(KeyVal=='1')
		{
			KeyVal=0;
			if(Time<3600)
				Time+=10;
		}
		if(KeyVal=='2')
		{
			KeyVal=0;
			if(Time>10)
				Time-=10;
		}
		LCD_Ref();
		delay_ms(50);  
		if(KeyVal=='E')
		{
			KeyVal=0;
			break;
		}
	}
	DOUBLE_EN=0;
	LCD_Clr();	
}



void PageDisInit(void)
{
	LCD_DispStr( 0,(unsigned char*)"�¶�:");
	LCD_DispStr(16,(unsigned char*)"ʪ��:");
	LCD_DispStr(32,(unsigned char*)"�趨:");
	LCD_DispStr(48,(unsigned char*)"����ʱ:");
}


void GetTempHumi(void)   //
{
	static unsigned char con=0;
	con++;
	if(con>=18)
	{
		con=0;
		DHT11_ReadData(&Temp,&Humi);
	}
}

 

void PageDisplay(void)
{
	TempDisplay(6,Temp);
	HumiDisplay(22,Humi);
	DisplayU16(40,Time);
	DisplayU16(56,CountTime);
}






  












