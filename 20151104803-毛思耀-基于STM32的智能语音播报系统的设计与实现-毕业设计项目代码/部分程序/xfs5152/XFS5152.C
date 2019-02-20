
#include "config.h"



/**************芯片设置命令*********************/
uint8_t XFS_StopCom[]={0xFD,0X00,0X01,0X02};//停止合成
uint8_t XFS_SuspendCom[]={0XFD,0X00,0X01,0X03};//暂停合成
uint8_t XFS_RecoverCom[]={0XFD,0X00,0X01,0X04};//恢复合成
uint8_t XFS_ChackCom[]={0XFD,0X00,0X01,0X21};//状态查询
uint8_t XFS_PowerDownCom[]={0XFD,0X00,0X01,0X88};//进入POWER DOWN 状态命令

/***********************************************/



/***********************************************************
* 名    称：  YS-XFS5051 文本合成函数
* 功    能：  发送合成文本到XFS5051芯片进行合成播放
* 入口参数：  *HZdata:文本指针变量 
* 出口参数：
* 说    明： 本函数只用于文本合成，具备背景音乐选择。默认波特率9600bps。					 
* 调用方法：例： 
**********************************************************/
void XFS_FrameInfo(uint8_t *HZdata)
{
/****************需要发送的文本**********************************/ 
		 unsigned  char xdata Frame_Info[50]; //定义的文本长度
         unsigned  int  HZ_Length;  

	     unsigned  int i=0; 
		 HZ_Length =strlen(HZdata); 			//需要发送文本的长度
 		 
/*****************帧固定配置信息**************************************/           
		 Frame_Info[0] = 0xFD ; 			//构造帧头FD
		 Frame_Info[1] = 0x00 ; 			//构造数据区长度的高字节
		 Frame_Info[2] = HZ_Length+2; 		//构造数据区长度的低字节
		 Frame_Info[3] = 0x01 ; 			//构造命令字：合成播放命令		 		 
		 Frame_Info[4] = 0x01;       //文本编码格式：GBK 
 
/*******************发送帧信息***************************************/		  
         memcpy(&Frame_Info[5], HZdata, HZ_Length);
		 PrintCom(Frame_Info,5+HZ_Length); //发送帧配置
}


/***********************************************************
* 名    称： void  main(void)
* 功    能： 主函数	程序入口
* 入口参数： *Info_data:固定的配置信息变量 
* 出口参数：
* 说    明：本函数用于配置，停止合成、暂停合成等设置 ，默认波特率9600bps。					 
* 调用方法：通过调用已经定义的相关数组进行配置。 
**********************************************************/
void YS_XFS_Set(uint8_t *Info_data)
{
	uint8_t Com_Len;
	Com_Len =strlen(Info_data);
	PrintCom(Info_data,Com_Len);
}




