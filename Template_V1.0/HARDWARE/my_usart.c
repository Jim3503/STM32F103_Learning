#include "my_usart.h"
#include "my_adc.h"


/*��������_IN_FLOATING
����������_IPU
����������_IPD
ģ������_AIN
��©���_OUT_OD
�������_OUT_PP
��©�������_AF_OD
���츴�����_AF_PP*/

void My_Usart_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	//����ʱ�Ӻ͸���GPIOʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE); //ʹ�� PA,USART1 �˿�ʱ��
	//PA9,IO��ʼ��
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//PA10��IO��ʼ��
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//���ڲ�����ʼ��
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART1,&USART_InitStruct);
	//����ʹ��
	USART_Cmd(USART1,ENABLE);
	
	//��������Щ��Ҫ����������Ӧ�жϣ���ʱ����main�����������NVIC_PriorityGroupConfig(NVIC_PriorityGroup_x);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);			//�򿪽����ж�
	
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;			//�����ʼ�������ĸ��жϣ�������ǿ����� stm32f10x.h ���ҵ�
															//ÿ���ж϶�Ӧ�����֡����� USART1_IRQn��
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			//���ж��Ƿ�ʹ�ܡ�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =1;   //��������жϵ���ռ���ȼ���
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;		    //��������жϵ������ȼ���
	NVIC_Init(&NVIC_InitStruct);
}

//�������ж�֮��ʼд�жϷ�����������������ļ�����ģ�ÿ�ν��յ����ݾͿ�ʼִ���жϷ�����
//��ȡ��Ӧ���ж�״̬�������жϷ����˾ͻ�����״̬�Ĵ����е�ĳ����־λ��
//�������жϷ��������棬Ҫ�жϸ��ж��������жϣ�ʹ�õĺ�������Ӧ�����GetITStatus����
//USART_ReceiveData()������stm32f10x_usart.h�ļ�����Ѱ��
////
//void USART1_IRQHandler(void)
//{
//	u8 res;
//	if(USART_GetITStatus(USART1,USART_IT_RXNE))
//	{
//		res = USART_ReceiveData(USART1);
//		USART_SendData(USART1,res);
//	}
//}

