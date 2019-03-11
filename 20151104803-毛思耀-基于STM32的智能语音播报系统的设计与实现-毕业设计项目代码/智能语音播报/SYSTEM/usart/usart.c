#include "sys.h"
#include "usart.h"	
#include "led.h"



unsigned char U1_Buf[1200]={0};
unsigned int  U1_Con=0;
unsigned char U1_Continue=0;
unsigned char U1_RxDone=0;



void uart_init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
     //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //Usart1 NVIC ����

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

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





void USART1_IRQHandler(void)                	//����1�жϷ������
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)   
	{
/*--------------------------------------------------------------------------*/
	  	U1_Buf[U1_Con++] = USART_ReceiveData(USART1);     //(USART1->DR);	//��ȡ���յ�������

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
    GPIO_InitTypeDef GPIO_InitStructure;//I/O��3�꼻���11��
    NVIC_InitTypeDef NVIC_InitStructure;//�D��3�꼻���11��
    USART_InitTypeDef USART_InitStructure;//'���3�꼻���11��  
    
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
    
    USART_InitStructure.USART_BaudRate = bound;                //�������a9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//��3��a8λ�y�Y���
    USART_InitStructure.USART_StopBits = USART_StopBits_1;     //���?�1λ
    USART_InitStructure.USART_Parity = USART_Parity_No;        //�T��żD?��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Y������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ?�?�   
    
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

void USART2_IRQHandler(void)                	//����1�жϷ������
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)   
	{
/*--------------------------------------------------------------------------*/
		rxdata = USART_ReceiveData(USART2);     //(USART1->DR);	//��ȡ���յ�������
/*---------------------------------------------------------------------------*/	
		USART_ClearFlag(USART2,USART_FLAG_RXNE); 
  } 
}

 













	

