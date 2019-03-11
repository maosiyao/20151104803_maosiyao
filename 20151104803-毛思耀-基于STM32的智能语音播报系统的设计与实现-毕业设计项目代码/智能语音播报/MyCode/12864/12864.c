#include "12864.h"
#include "delay.h"


u8 LCD_Buf[64]={0};
u8 LCD_RefFlag = 0;

//CS   PB3
//SID  PB4
//CLK  PB5
#define LCD_CLK_SET    		GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define LCD_CLK_RESET 	 	GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define LCD_SID_SET    		GPIO_SetBits(GPIOB,GPIO_Pin_4)
#define LCD_SID_RESET  		GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define LCD_CS_SET     		GPIO_SetBits(GPIOB,GPIO_Pin_3)
#define LCD_CS_RESET   		GPIO_ResetBits(GPIOB,GPIO_Pin_3)

static void LCD_delay(void)
{
	delay_us(2);
}

static  void LCD_Write_Byte(u8 Data)   
{
	u8 i;
	for(i=0;i<8;i++)
	{
		LCD_delay();
		LCD_CLK_RESET;        
		if(Data & 0x80)
			LCD_SID_SET;  
		else
			LCD_SID_RESET; 
		LCD_delay();
		LCD_CLK_SET; 	
		Data = Data<<1;    
	} 
}

static void LCD_Write_Command(u8 COMMAND)   
{
	LCD_CS_SET;
	LCD_Write_Byte(SENDCOMM); 
	LCD_delay();
	LCD_Write_Byte(COMMAND & 0XF0);   
	LCD_delay();
	LCD_Write_Byte((COMMAND << 4) & 0XF0);   
	LCD_delay();   
	LCD_CS_RESET;
}

static void LCD_Write_Data(u8 Data)      
{
	LCD_CS_SET;
	LCD_Write_Byte(SENDDATA);  
	LCD_delay();  
	LCD_Write_Byte(Data & 0XF0);
	LCD_delay();
	LCD_Write_Byte((Data << 4)&0XF0);     
	LCD_delay();
	LCD_CS_RESET;
}

static void LCD_XY(u8 x,u8 y)      //X(1~16),Y(1~4)
{
	switch(y)
	{
		case 1:LCD_Write_Command(0X7F+x);break;
		case 2:LCD_Write_Command(0X8F+x);break;
		case 3:LCD_Write_Command(0X87+x);break;
		case 4:LCD_Write_Command(0X97+x);break;
		default: break;
	}
}

static void LCD_PortInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	 //ʹ��PB

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.12
	GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);						 //PA.12 �����	
}


//LCD��ʾ��ʼ��
void LCD_Init(void)
{
	LCD_PortInit(); 
	LCD_Write_Command(0x34); 
	delay_ms(100);
	LCD_Write_Command(0x30); 
	delay_ms(100);
	LCD_Write_Command(0x0C);
	delay_ms(100);
	LCD_Write_Command(0x01); 
	delay_ms(100);
	LCD_Clr();  
}


//��ʾһ���ַ�
//add:��ʾ��ַ0-63��Ӧadd/16��add%16�е�λ��
void LCD_DispChar(unsigned char add,unsigned char c)
{
	if(add>63)
		return;
	LCD_Buf[add]=c;
	LCD_RefFlag = 1;
}


//��ʾһ���ַ���
//add:��ʾ��ַ0-63��Ӧadd/16��add%16�е�λ��
//�ر�������ʾ����addһ��Ҫ��ż��
void LCD_DispStr(unsigned char add,unsigned char *str)
{
	while(*str!='\0')
	{
		LCD_Buf[add]=*str;
		add++;
		if(add>63)
			return;
		str++;
	}
	LCD_RefFlag = 1;
}




//�¶���ʾ
void TempDisplay(unsigned char add,unsigned char temp)
{
	unsigned char DisBuff[6]={0};
 
    DisBuff[0] = temp / 10+'0';  //??
    DisBuff[1] = temp % 10+'0';    //??
  	DisBuff[2]='\'';
  	DisBuff[3]='C';
  	DisBuff[4]='\0';
	if(DisBuff[0]=='0')
	{
		DisBuff[0]=0x20;
	}
  	LCD_DispStr(add,DisBuff);
}

//ʪ����ʾ
void HumiDisplay(unsigned char add,unsigned char humi)
{
	unsigned char DisBuff[8]={0};
  
	DisBuff[0] = humi / 10+'0';  //??
    DisBuff[1] = humi % 10+'0';    //??
  	DisBuff[2]= '%';
	DisBuff[3]= 'R';
	DisBuff[4]= 'H';
  	DisBuff[5]='\0';
	if(DisBuff[0]=='0')
	{
		DisBuff[0]=0x20;
	}
  	LCD_DispStr(add,DisBuff);
}



//ȫ����������
void LCD_Clr(void)
{
	unsigned char i = 0;
	for(i=0;i<64;i++)
	{
		LCD_Buf[i]=0x20;
	}
	LCD_RefFlag = 1; 
	LCD_Ref();	
}


//��ʾ��ˢ�º���
void LCD_Ref(void)   //��ʾˢ��
{
	unsigned char i = 0;
	
	if(LCD_RefFlag)
	{
		LCD_RefFlag = 0;	
		LCD_XY(1,1);
		for(i=0;i<64;i++)
		{
			LCD_delay();
			if(i==16)
				LCD_XY(1,2); 
			else if(i==32)
				LCD_XY(1,3);
			else if(i==48)
				LCD_XY(1,4);
			LCD_Write_Data(LCD_Buf[i]);
		}
	}
}



 


void DisplayU16(unsigned char add,unsigned int dat)
{
  unsigned char buf[8]={0};//12345
	unsigned char i=0;
	buf[0]=dat/10000+'0';   //��λ
	dat=dat%10000;
	buf[1]=dat/1000+'0';  //ǧλ
	dat=dat%1000;
	buf[2]=dat/100+'0';      //��λ
	dat=dat%100;
	buf[3]=dat/10+'0';  //ʮλ
	buf[4]=dat%10+'0';  //��λ
	buf[5]='S';  //��λ
	buf[6]='\0';
  
	for(i=0;i<3;i++)
	{
	  if(buf[i]=='0')
			buf[i]=0x20; 
    else
      break;			
	}
	LCD_DispStr(add,buf);
}

 




























