
#ifndef __XFS5152_H
#define __XFS5152_H
#include "stdio.h"	
#include "sys.h" 


void PrintCom(u8 *DAT,u16 len);
void XFS_Start(unsigned char *HZdata);


void XFS_Suspend(void);//��ͣ�ϳ�
void XFS_Recover(void);//�����ϳ�
void XFS_Stop(void);  //ֹͣ�ϳ�


#endif
