#include "key4.h"




//PB0  PB1


unsigned char KeyVal = 0;
unsigned char KeyState = KEY_IDLE;
unsigned char KeyTime = 1;  

#define KEY1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)
#define KEY2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)
#define KEY3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
#define KEY4  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)

void KEY_PortInit(void)
{

  GPIO_InitTypeDef   GPIO_InitStructure;	
  //PA7
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1 | GPIO_Pin_10|GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
}



void KEY_Scan(void)	 //20-50MS?????
{
  switch(KeyState)
  {
		case KEY_IDLE:	 //???????? 
			{
				if(KEY1==0||KEY2==0 ||KEY3==0 ||KEY4==0)
					KeyState=KEY_ENSURE;
				else
					KeyState=KEY_IDLE;
			}break;
		case KEY_ENSURE:  //????? ?????
			{
				if(KEY1==0||KEY2==0||KEY3==0 ||KEY4==0)
					KeyState=KEY_PRESS;
				else
					KeyState=KEY_IDLE;	
			}break;
		case KEY_PRESS:		 //
			{
				if(KEY1==0)
					KeyVal = '+';
				else if(KEY2==0)
					KeyVal = '-';
				else if(KEY3==0)
					KeyVal = '1';
				else if(KEY4==0)
					KeyVal='0';
				else
					KeyState=KEY_IDLE;	  
				if(KeyState==KEY_PRESS)
				{
					KeyState=KEY_WAIT_UP;
				}
/*--------------------------------------------------------*/
		 if((KeyVal=='+')||(KeyVal=='-'))
		 {
		   KeyTime = 1;
		 }
		 else
		 {
		   KeyTime = 0;
		 }

/*----------------------------------------------------------*/
		}break;
	case KEY_WAIT_UP:
	   {
		 if(KeyTime>0)
		 {
		   KeyTime++;
		   if(KeyTime>=4)
		   {
		     KeyTime = 1;
			 KeyState=KEY_IDLE;
		   }
		 }
		 else
		 {
		   if(KEY1 && KEY2 && KEY3  && KEY4)
		   {
		     KeyState=KEY_IDLE;
		     KeyTime = 1; 
		   }
		   else
		     KeyState=KEY_WAIT_UP;  
		 }
	   }break;
	default:
	   {
	     KeyState=KEY_IDLE;
	   }break;
  }
}




































