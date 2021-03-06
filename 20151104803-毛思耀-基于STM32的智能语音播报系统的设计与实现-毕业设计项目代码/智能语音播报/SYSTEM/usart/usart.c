#include "sys.h"
#include "usart.h"	
#include "led.h"



unsigned char U1_Buf[1200]={0};
unsigned int  U1_Con=0;
unsigned char U1_Continue=0;
unsigned char U1_RxDone=0;



void uart_init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //Usart1 NVIC 配置

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART1, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART1, ENABLE);                    //使能串口 

}



void Uart1_SendOneByte(u8 dat)
{
  USART_SendData(USART1,dat); 
  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
  USART_ClearFlag(USART1,USART_FLAG_TC);
}





void UART1_SendStr(u8* str)
{
	while(*str)
	{
		Uart1_SendOneByte(*str);
		str++;
	}
}


unsigned char tremp=0;





void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)   
	{
/*--------------------------------------------------------------------------*/
	  	U1_Buf[U1_Con++] = USART_ReceiveData(USART1);     //(USART1->DR);	//读取接收到的数据

		if(U1_Con>=1200)
		{
			U1_Con=0;
			U1_RxDone = 1;
		}
		U1_Continue=1;
/*---------------------------------------------------------------------------*/	
		USART_ClearFlag(USART1,USART_FLAG_RXNE); 
  } 
}



void USART2_Init( u32 bound )
{
    GPIO_InitTypeDef GPIO_InitStructure;//I/O窥3蹶蓟�11戾
    NVIC_InitTypeDef NVIC_InitStructure;//諨断3蹶蓟�11戾
    USART_InitTypeDef USART_InitStructure;//'�3蹶蓟�11戾  
    
    /* Enable the USART2 Pins Software Remapping */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 
    
    
    /* Configure USART2 Rx (PA.03) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* Configure USART2 Tx (PA.02) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* Enable the USART2 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);    
    
    USART_InitStructure.USART_BaudRate = bound;                //蚧般殍置蝍9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字3のa8位陏綴格杲
    USART_InitStructure.USART_StopBits = USART_StopBits_1;     //蚧个�?�1位
    USART_InitStructure.USART_Parity = USART_Parity_No;        //蜹奇偶D?验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Y狩控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//暾�?�?杲   
    
    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    //USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
    /* Enable USART2 */
    USART_Cmd(USART2, ENABLE);
}


void USART2_SendByte(u8 dat)
{
  USART_SendData(USART2,dat); 
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
	USART_ClearFlag(USART2,USART_FLAG_TC);
}

unsigned char rxdata=0;

void USART2_IRQHandler(void)                	//串口1中断服务程序
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)   
	{
/*--------------------------------------------------------------------------*/
		rxdata = USART_ReceiveData(USART2);     //(USART1->DR);	//读取接收到的数据
/*---------------------------------------------------------------------------*/	
		USART_ClearFlag(USART2,USART_FLAG_RXNE); 
  } 
}

 













	

