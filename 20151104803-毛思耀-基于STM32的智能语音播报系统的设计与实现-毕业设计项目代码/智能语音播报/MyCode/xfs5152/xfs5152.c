#include "sys.h"
#include "usart.h"	
#include "string.h"


 
u8  XFS_StopCom[]={0xFD,0X00,0X01,0X02};     //ֹͣ�ϳ�
u8  XFS_SuspendCom[]={0XFD,0X00,0X01,0X03};  //��ͣ�ϳ� 	
u8  XFS_RecoverCom[]={0XFD,0X00,0X01,0X04};  //�ָ��ϳ�



void PrintCom(u8 *DAT,u16 len)
{
	u16 i;
	for(i=0;i<len;i++)
	{
		USART2_SendByte(*DAT++);
	}	
}



void XFS_Start(unsigned char *HZdata)
{
/*************************************************/ 
//		 unsigned  char  Frame_Info[500];  
         unsigned  int   HZ_Length;  
		 unsigned  int i=0;
	
		 HZ_Length =strlen((const char*)HZdata); 	//��Ҫ���͵��ı�����
	
		USART2_SendByte(0xFD);
		USART2_SendByte((HZ_Length+2)/256);	
		USART2_SendByte((HZ_Length+2)%256);	
		USART2_SendByte(0x01);		
		USART2_SendByte(0x01);
		for(i=0;i<HZ_Length;i++)
		{
			USART2_SendByte(*HZdata);
			HZdata++;
		}
}



//��ͣ�ϳ�
void XFS_Suspend(void)
{
	u8 i=0;
	for(i=0;i<4;i++)
	{
		USART2_SendByte(XFS_SuspendCom[i]);
	}
}

//�����ϳ�
void XFS_Recover(void)
{
	u8 i=0;
	for(i=0;i<4;i++)
	{
		USART2_SendByte(XFS_RecoverCom[i]);
	}
}


void XFS_Stop(void)
{
	u8 i=0;
	for(i=0;i<4;i++)
	{
		USART2_SendByte(XFS_StopCom[i]);
	}
}





















