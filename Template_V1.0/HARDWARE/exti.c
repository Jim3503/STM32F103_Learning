#include "exti.h"
#include "delay.h"
#include "key.h"
#include "led.h"
/*ע��˴�������ѧϰkey0h��key1�������ⲿ�жϵ�ԭ��*/
void EXITX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	//��ʼ��GPIO��Ϊ����
	//��߿�����Ӱ����ĳ�ʼ���������δ��д�����key.c�ļ�key.Init()�����Ѿ���GPIO�ĳ�ʼ������
	//key_Init()
	
	//����IO����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//����IO�ں��ж��ߵ�ӳ���ϵ��key0��key1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
	//��ʼ�������жϣ����ô�������
	EXTI_InitStruct.EXTI_Line = EXTI_Line4;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line3;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
}
//��д�жϷ��������жϷ������ĸ�ʽ�������ļ�����
void EXTI4_IRQHandler(void)
{
	delay_ms(10);
	if(KEY0 == 0)
	{
		LED0 = !LED0;
		LED1 = !LED1;
	}
	
	//����жϱ�־λ
	EXTI_ClearITPendingBit(EXTI_Line4);
}
//��д�жϷ��������жϷ������������ļ�����
void EXTI3_IRQHandler(void)
{
	delay_ms(10);
	if(KEY1 == 0)
	{
		LED0 = !LED0;
		LED1 = !LED1;
	}
	
	//����жϱ�־λ
	EXTI_ClearITPendingBit(EXTI_Line3);
}

