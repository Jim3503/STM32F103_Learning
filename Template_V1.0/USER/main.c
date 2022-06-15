#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "buzzer.h"
#include "my_usart.h"
#include "key.h"
#include "exti.h"
#include "my_adc.h"
#include "usart.h"
#include "t_adc.h"
#include "light_adc.h"
#include "my_dma.h"
#include "my_iic.h"
#include "24cxx.h"
#include "my_can.h"

u16 temp;
u16 temp_light;

#define SEND_BUF_SIZE 8200
u8 SendBuff[SEND_BUF_SIZE]; //�������ݻ�����
const u8 TEXT_TO_SEND[]={"ALIENTEK ELITE STM32F1 DMA ����ʵ��"};

//Ҫд�뵽24c02���ַ�������
const u8 TEXT_Buffer[]={"Elite STM32 IIC TEST"};
#define SIZE sizeof(TEXT_Buffer)	

int main(void)
{
	u16 i;
	u8 j,mask=0;
	u8 t=0;
	float pro=0;//����
	u8 datatemp[SIZE];
	u8 canbuf[8] = {'1','2','3','4','5','6','\r','\n'};		//����һ��CAN�շ����ݵĻ�����
	u8 mode = CAN_Mode_LoopBack;	//CAN����ģʽ;CAN_Mode_Normal(0)����ͨģʽ��
									//CAN_Mode_LoopBack(1)������ģʽ
	u8 res;
	u8 canbuf1[8];
	u8 temp;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
	//LED_Init();
	//My_Usart_Init();
	//Buzzer_Init();
	KEY_Init();
	EXITX_Init();
	//My_ADC_Init();
	uart_init(115200);
	T_ADC_Init();
	Light_ADC_Init();
	AT24CXX_Init(); //IIC ��ʼ��
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);//CAN��ʼ������ģʽ,������500Kbps    

/********************************DMA����ʵ��******************************************	
	MYDMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)SendBuff,SEND_BUF_SIZE);
	
	j=sizeof(TEXT_TO_SEND); 
		for(i=0;i<SEND_BUF_SIZE;i++)//������ݵ� SendBuff
		{
			if(t>=j)//���뻻�з�
			{
				if(mask)
				{
				SendBuff[i]=0x0a;
				t=0;
				}else
				{
					SendBuff[i]=0x0d;
					mask++;
				}
			}else//���� TEXT_TO_SEND ���
			{
				mask=0;
				SendBuff[i]=TEXT_TO_SEND[t];
				t++;
			}
		}
*/
//******************************************************************
	 while(1)
		{
//*******************GPIO_SetBits��GPIO_ResetBits��ʹ��***********************************************
//			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
//			GPIO_SetBits(GPIOE,GPIO_Pin_5);
//			delay_ms(300);
//			//Delay(3000000);
//			GPIO_SetBits(GPIOB,GPIO_Pin_5);
//			GPIO_ResetBits(GPIOE,GPIO_Pin_5);
//			delay_ms(300);
//			//Delay(3000000);
//*******************GPIO_SetBits��GPIO_ResetBits��ʹ��***********************************************

//*******************�����led.h������Ǹ�#define LED0 PBout(5)// DS0��ͨ��λ��������IO�Ŀ���***********************************************
//			LED0=0; 
//			LED1=1;
//			delay_ms(1000); //��ʱ 300ms
//			LED0=1;
//			LED1=0;
//			delay_ms(1000); //��ʱ 300ms
//*******************led.h������Ǹ�#define LED0 PBout(5)// DS0��ͨ��λ��������IO�Ŀ���***********************************************
//����IO֮ǰ�ȵö�ʱ��ʹ�ܣ��ٶ�IO���г�ʼ��������ͨ���⺯��Ҳ����ͨ��λ�������߼Ĵ��������п���IO
/********************��������ʹ��**************************************************
//			Buzzer0 = 1;
//			delay_ms(1000); //��ʱ 300ms
//			Buzzer0 = 0;
//			delay_ms(1000); //��ʱ 300ms
////			GPIO_ResetBits(GPIOB,GPIO_Pin_8);
////			delay_ms(500);
////			GPIO_SetBits(GPIOB,GPIO_Pin_8);
////			delay_ms(500);
**************************************************************************/

/*************************��ȡADC1(PA1)ת����ֵ���������ô���printf�������*******/
//	temp = Get_ADC(1);
//	temp=(float)temp*(3.3/4096);
//	printf( "�ɼ����ĵ�ѹֵ��%d V \n",temp);
/******************************************************************************/

/******************************�ɼ��ڲ��¶�*****************************************/
		temp = Get_Temprate();
		printf("�ڲ��¶ȣ�%d \r\n",temp/100);
/******************************�ɼ��ڲ��¶�*****************************************/

/******************************�ɼ�����ǿ��*****************************************/
		temp_light = Lsens_Get_Val();
		printf("����ǿ��: %d \r\n",temp_light);
/******************************�ɼ�����ǿ��*****************************************/
/*****************************DMA����ʵ��*****************************************/
/*
USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
		//ʹ�ܴ��� 1 �� DMA ����
		MYDMA_Enable(DMA1_Channel4);//��ʼһ�� DMA ���䣡
		
		while(1)
		{
			if(DMA_GetFlagStatus(DMA1_FLAG_TC4)!=RESET)//�ж�ͨ�� 4 �������
			{
				DMA_ClearFlag(DMA1_FLAG_TC4);//���ͨ�� 4 ������ɱ�־
				break; 
			}
			pro=DMA_GetCurrDataCounter(DMA1_Channel4);//�õ���ǰʣ��������
			pro=1-pro/SEND_BUF_SIZE;//�õ��ٷֱ�
			pro*=100; //���� 100 ��
			printf("������ȣ�%f",pro);
		}
*/		
/******************************DMA����ʵ��*****************************************/

/***********************************IICʵ��****************************************/
	while(AT24CXX_Check())//��ⲻ��24c02
	{
		LED0=!LED0;//DS0��˸
	}
	AT24CXX_Write(0,(u8*)TEXT_Buffer,SIZE);
	AT24CXX_Read(0,datatemp,SIZE);
	//printf("%s",datatemp);
/***********************************IICʵ��****************************************/
	
/***********************************CANʵ��****************************************/
	res=Can_Send_Msg(canbuf,8);//����8���ֽ� ,���ͳɹ���1������ʧ����0
	if(res) printf("���ͳɹ�\r\n");
	temp = Can_Receive_Msg(canbuf1);
	if(temp)
	{
		printf("���յ�����:%s",canbuf1);
	}


}
}
