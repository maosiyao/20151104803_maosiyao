#include "dht11.h"
#include "delay.h"
#include "stm32f10x.h"



#define DHT11_0    GPIO_ResetBits(GPIOA,GPIO_Pin_0)
#define DHT11_1    GPIO_SetBits(GPIOA,GPIO_Pin_0)  
#define DHT11_Val  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)


//DHT11  PA0
void DHT11_DQ_OUT(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

	//  PA0
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
  GPIO_SetBits(GPIOA,GPIO_Pin_0); 
}


void DHT11_DQ_IN(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
}



//复位DHT11 返回1 DHT11 不存在  返回0 收到应答信号
u8 DHT11_Check(void)
{
  u8 retry=0;
	
	DHT11_DQ_OUT();
	DHT11_0;
	delay_ms(20);
  DHT11_1;
	DHT11_DQ_IN();
	delay_us(40);   //延时40us检测
	
	while(DHT11_Val && retry < 50) //
	{
		retry ++;
		delay_us(5);
	} 
	if(retry>=50)
		return 1;
	else 
		retry=0;
/*------------------------------------------------------*/
	while(!DHT11_Val && retry<50)
	{
	  retry++;
		delay_us(5);
	}
	if(retry>=50)
		return 1;
	return 0;
}


//从DHT11 读取一个字节函数
//返回 读取的数据

u8 DHT11_ReadByte(void)
{
  u8 i,dat=0;
	u16 retry=0;
	
	for(i=0;i<8;i++)
	{
	  retry=0;
		while(DHT11_Val)   //等待上一字节高电平结束
	  {
		  if(retry++>500)
				break;
		}
		retry=0;
		
		while(0==DHT11_Val)  //等待低电平结束
		{
		  if(retry++>500)
				break;
		}
		delay_us(30);
		dat<<=1;
		if(DHT11_Val)
			dat|=0x01;	
	}
	return dat;
}



u8 DHT11_ReadData(u8 *temp,u8 *humi)
{
  u8 buf[5]={0};
	u8 i=0;
	
	if(0==DHT11_Check())
	{
	  for(i=0;i<5;i++)
		{
		  buf[i]=DHT11_ReadByte();
		}
		
		if((buf[0]+buf[1]+buf[2]+buf[3]) == buf[4])
		{
            *humi = buf[0] % 100;
            *temp = buf[2] % 100;            
            return 0;
		}	
	
	}
	return 1;
}



























