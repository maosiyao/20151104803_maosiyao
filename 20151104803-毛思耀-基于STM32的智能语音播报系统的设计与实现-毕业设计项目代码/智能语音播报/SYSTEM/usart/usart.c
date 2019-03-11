#include "sys.h"
#include "usart.h"	
#include "led.h"



unsigned char U1_Buf[1200]={0};
unsigned int  U1_Con=0;
unsigned char U1_Continue=0;
unsigned char U1_RxDone=0;



void uart_init(u32 bound)
{
    //GPIO¶Ë¿ÚÉèÖÃ
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//Ê¹ÄÜUSART1£¬GPIOAÊ±ÖÓ
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//¸´ÓÃÍÆÍìÊä³ö
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//¸¡¿ÕÊäÈë
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //Usart1 NVIC ÅäÖÃ

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷
  
   //USART ³õÊ¼»¯ÉèÖÃ

	USART_InitStructure.USART_BaudRate = bound;//Ò»°ãÉèÖÃÎª9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êý¾Ý¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;//ÎÞÆæÅ¼Ð£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎÞÓ²¼þÊý¾ÝÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê½

    USART_Init(USART1, &USART_InitStructure); //³õÊ¼»¯´®¿Ú
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//¿ªÆôÖÐ¶Ï
    USART_Cmd(USART1, ENABLE);                    //Ê¹ÄÜ´®¿Ú 

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





void USART1_IRQHandler(void)                	//´®¿Ú1ÖÐ¶Ï·þÎñ³ÌÐò
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)   
	{
/*--------------------------------------------------------------------------*/
	  	U1_Buf[U1_Con++] = USART_ReceiveData(USART1);     //(USART1->DR);	//¶ÁÈ¡½ÓÊÕµ½µÄÊý¾Ý

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
    GPIO_InitTypeDef GPIO_InitStructure;//I/O¿ú3õê¼»¯½á11ìå
    NVIC_InitTypeDef NVIC_InitStructure;//ÖD¶Ï3õê¼»¯½á11ìå
    USART_InitTypeDef USART_InitStructure;//'®¿ú3õê¼»¯½á11ìå  
    
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
    
    USART_InitStructure.USART_BaudRate = bound;                //ò»°ãéèÖÃÎa9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö3¤Îa8Î»êy¾Y¸ñê½
    USART_InitStructure.USART_StopBits = USART_StopBits_1;     //ò»¸öí?Ö1Î»
    USART_InitStructure.USART_Parity = USART_Parity_No;        //ÎTÆæÅ¼D?ÑéÎ»
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Yá÷¿ØÖÆ
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//êÕ·?Ä?ê½   
    
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

void USART2_IRQHandler(void)                	//´®¿Ú1ÖÐ¶Ï·þÎñ³ÌÐò
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)   
	{
/*--------------------------------------------------------------------------*/
		rxdata = USART_ReceiveData(USART2);     //(USART1->DR);	//¶ÁÈ¡½ÓÊÕµ½µÄÊý¾Ý
/*---------------------------------------------------------------------------*/	
		USART_ClearFlag(USART2,USART_FLAG_RXNE); 
  } 
}

 













	

