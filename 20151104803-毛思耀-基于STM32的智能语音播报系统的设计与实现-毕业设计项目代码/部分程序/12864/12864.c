#include "12864.h"
#include "delay.h"


u8 LCD_Buf[64]={0};
u8 SparkF = 0;    //????
u8 SparkA = 0;    //??????
u8 SparkL = 1;    //??????1
u8 LCD_RefFlag = 0;


void LCD_Write_Byte(u8 Data)   
{
  u8 i;
  for(i=0;i<8;i++)
  {
    delay_us(2);
		LCD_CLK_RESET;        
		if(Data & 0x80)
    { LCD_SID_SET;  }
    else
    { LCD_SID_RESET; }
    delay_us(2); 	
		LCD_CLK_SET; 	
    Data = Data<<1;    
  } 
}

void LCD_Write_Command(u8 COMMAND)   
{
  LCD_CS_SET;
  LCD_Write_Byte(SENDCOMM); 
  delay_us(2);
  LCD_Write_Byte(COMMAND & 0XF0);   
  delay_us(2);
  LCD_Write_Byte((COMMAND << 4) & 0XF0);   
  delay_us(2);   
  LCD_CS_RESET;
}

void LCD_Write_Data(u8 Data)      
{
  LCD_CS_SET;
  LCD_Write_Byte(SENDDATA);  
  delay_us(2);   
  LCD_Write_Byte(Data & 0XF0);
  delay_us(2);   
  LCD_Write_Byte((Data << 4)&0XF0);     
  delay_us(2); 
	LCD_CS_RESET;
}

void LCD_XY(u8 x,u8 y)      //??????    X(1~16),Y(1~4)
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

void LCD_PortInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	 //使能PB
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;				 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.12
  GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);						 //PA.12 输出高
	
}



void LCD_Init(void)
{
  LCD_PortInit(); 
  LCD_Write_Command(0x34);//???8???
	delay_ms(20);
  LCD_Write_Command(0x30);//??????
	delay_ms(20);
  LCD_Write_Command(0x0C);//????,??off,
	delay_ms(20);
	LCD_Write_Command(0x01); 
  LCD_Clr();  
  delay_ms(100);
}

void Display_Char(u8 x,u8 y, u8 Char)
{
  u8 add = 2*(x-1)+16*(y-1);
  LCD_Buf[add]=Char;
  LCD_RefFlag = 1;
}

void DisplayOneChar(u8 add,u8 ch)
{
  LCD_RefFlag = 0;
  LCD_Buf[add]=ch;
  LCD_RefFlag = 1;
}

void Display_String(u8 x,u8 y,u8 *string)
{
  u8 add = 2*(x-1)+16*(y-1);
  LCD_RefFlag = 0;
  while(*string!='\0')
  {
    LCD_Buf[add]=*string;
	add++;
	string++;
  }
  LCD_RefFlag = 1;
}




 

void LCD_Clr(void)
{
  unsigned char i = 0;
  LCD_RefFlag = 0;
  for(i=0;i<64;i++)
  {
    LCD_Buf[i]=0x20;
  }
  LCD_RefFlag = 1;   
}


void DisplayU16(unsigned char x,unsigned char y,unsigned int dat)
{
  unsigned char buf[7]={0};//12345
	unsigned char i=0;
//	buf[0]=dat/10000+'0';   //万位
	dat=dat%10000;
	buf[0]=dat/1000+'0';  //千位
	dat=dat%1000;
	buf[1]=dat/100+'0';      //百位
	dat=dat%100;
	buf[2]=dat/10+'0';  //十位
	buf[3]=dat%10+'0';  //个位
	buf[4]='\0';
  
	for(i=0;i<3;i++)
	{
	  if(buf[i]=='0')
			buf[i]=0x20; 
    else
      break;			
	}
	Display_String(x,y,buf);
}


void Display_u8(u8 x,u8 y,u16 num)
{
    u8 buff[4]={0};
	buff[0]=num / 100 +'0';
	buff[1]=num%100/10 +'0';
	buff[2]=num%10+'0';
	buff[3]='\0';
	if(buff[0]=='0')
	{
	  buff[0]=' ';
	  if(buff[1]=='0')
	    buff[1]=' ';
	}
	Display_String(x,y,buff);
}

 
void DHT_HumiDisplay(unsigned char add,unsigned char humi)
{
		unsigned char DisBuff[6]={0};
		DisBuff[0] = 'H';
		DisBuff[1] = ':';
    DisBuff[2] = humi % 100 / 10+'0';  //??
    DisBuff[3] = humi % 10+'0';    //??
  	DisBuff[4]= '%';
  	DisBuff[5]='\0';
	if(DisBuff[2]=='0')
	{
		DisBuff[2]=0x20;
	}
  	Display_String(6,1,DisBuff);
}

void TempDisplay(unsigned char add,float Temp)
{
  unsigned char DisBuff[10]={0};
  unsigned int Change = 0;
    Change = 10*Temp;  //??????10? 102.8   1028
    DisBuff[0] = 'T';
		DisBuff[1] = ':';
    DisBuff[2] = Change % 1000 / 100+'0';  //??
    DisBuff[3] = Change % 100 / 10+'0';    //??
    DisBuff[4] = '.';   
    DisBuff[5] = Change % 10+'0';   
//----------------???????----------------------------// 
    if(DisBuff[2]=='0')       //???0
    { 
      DisBuff[2] = 0x20;
    }   
  DisBuff[6]='\'';
  DisBuff[7]='C';
  DisBuff[8]='\0';
  Display_String(1,1,DisBuff);
}


//LCD行清屏 line 1-4
void LCD_LineClr(u8 line)
{
  unsigned char i = 0;
  unsigned char add=0;
  if((line==0)||(line>4))
    return;
  add=16*(line-1);
  LCD_RefFlag = 0;
  for(i=0;i<16;i++)
  {
    LCD_Buf[i+add]=0x20;
  }
  LCD_RefFlag = 1;   
}

void LCD_Ref(void)   //显示刷新
{
  unsigned char i = 0;
  LCD_XY(1,1);
  LCD_RefFlag = 0;
  for(i=0;i<64;i++)
  {
		 delay_us(10);  
		if(i==16)
       LCD_XY(1,2); 
	else if(i==32)
       LCD_XY(1,3);
	else if(i==48)
       LCD_XY(1,4);
	LCD_Write_Data(LCD_Buf[i]);
  }
  LCD_RefFlag = 1;
}


void LCD_Task(void)  //显示屏刷新函数20-50ms执行一次
{ 

/*-------------刷新功能-------------------*/
	if(LCD_RefFlag)
	{
	    LCD_RefFlag = 0;
	    LCD_Ref();
	}
}




 





































