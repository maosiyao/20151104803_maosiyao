#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "string.h" 


void uart3_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;	//�շ�ģʽ USART_Mode_Rx |

  USART_Init(USART3, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���1 

} 
/************************************************************************
���������� ���ڷ����ַ�������
��ڲ����� 	*DAT���ַ���ָ��
�� �� ֵ�� none
����˵���� API ���ⲿʹ�ã�ֱ�ۣ�
**************************************************************************/
void PrintCom(uint8_t *DAT,uint8_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)
	{
	   USART_SendData(USART3,*DAT++);//�򴮿�1��������
		 while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);/* ���ָ����USART��־λ��RESET��1ʱ�������*/
	}	
}

/***********************************************************
* ��    �ƣ�  YS-XFS5051 �ı��ϳɺ���
* ��    �ܣ�  ���ͺϳ��ı���XFS5051оƬ���кϳɲ���
* ��ڲ�����  *HZdata:�ı�ָ����� 
* ���ڲ�����
* ˵    ���� ������ֻ�����ı��ϳɣ��߱���������ѡ��Ĭ�ϲ�����9600bps��					 
* ���÷��������� SYN_FrameInfo���������Ƶ��ӡ�����
**********************************************************/
void XFS_FrameInfo(char *HZdata)
{ 
/****************��Ҫ���͵��ı�**********************************/ 
		unsigned  char  Frame_Info[50]; //������ı�����  xdata          
         unsigned  int  HZ_Length;  

//	     unsigned  int i=0; 
		 HZ_Length =strlen(HZdata); 			//��Ҫ�����ı��ĳ���
 		 
/*****************֡�̶�������Ϣ**************************************/           
		 Frame_Info[0] = 0xFD ; 			//����֡ͷFD
		 Frame_Info[1] = 0x00 ; 			//�������������ȵĸ��ֽ�
		 Frame_Info[2] = HZ_Length+2; 		//�������������ȵĵ��ֽ�
		 Frame_Info[3] = 0x01 ; 			//���������֣��ϳɲ�������		 		 
		 Frame_Info[4] = 0x01;       //�ı������ʽ��GBK 
 
/*******************����֡��Ϣ***************************************/		  
         memcpy(&Frame_Info[5], HZdata, HZ_Length);
		 PrintCom(Frame_Info,5+HZ_Length); //����֡����
}

 int main(void)
 {		

	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	 uart3_init(9600);
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	 
	 
	 
 	while(1)
	{
		delay_ms(1000);
     XFS_FrameInfo("ͬѧ������Ϸ��") ;

	}	 
 }

