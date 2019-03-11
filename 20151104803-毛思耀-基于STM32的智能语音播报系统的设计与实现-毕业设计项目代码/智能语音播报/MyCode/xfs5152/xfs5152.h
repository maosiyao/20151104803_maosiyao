
#ifndef __XFS5152_H
#define __XFS5152_H
#include "stdio.h"	
#include "sys.h" 


void PrintCom(u8 *DAT,u16 len);
void XFS_Start(unsigned char *HZdata);


void XFS_Suspend(void);//暂停合成
void XFS_Recover(void);//继续合成
void XFS_Stop(void);  //停止合成


#endif
