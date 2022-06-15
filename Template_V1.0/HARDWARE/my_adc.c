#include "my_adc.h"
#include "delay.h"
/*
1����ʼ��������ɿ���PA��ʱ�Ӻ�ADC1ʱ�ӣ�ͬʱ��GPIO���г�ʼ������PA1Ϊģ������
2����λADC1��ͬʱ����ADC1��Ƶ����
3����ʼ��ADC1����������ADC1�Ĺ���ģʽ�Լ��������е������Ϣ
4��ʹ��ADC��У׼
*/
void My_ADC_Init(void)//ADC��ʼ�� 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef  ADC_InitStruct;
	//����PA��ʱ�Ӻ�ADC1ʱ�ӣ�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
	//��ʼ��GPIOA������PA1Ϊģ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //-->PA.1 ģ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //ģ������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//����ADC1�ķ�Ƶ����Ϊ6����ΪADC�����Ƶ�ʲ��ܳ���14M��72M/6=12,
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	ADC_DeInit(ADC1);
	
	//��ʼ��ADC1
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;		//����ת��ģʽ
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;		//ADC �����Ҷ���
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//ת��������������ⲿ
																	//��������
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;					//ADC ����ģʽ
	ADC_InitStruct.ADC_NbrOfChannel = 0x1;				//˳����й���ת����ADC ͨ������Ŀ
	ADC_InitStruct.ADC_ScanConvMode = DISABLE; 			//��ͨ��ģʽ
	
	ADC_Init(ADC1,&ADC_InitStruct);
	
	//ʹ��ADC1
	ADC_Cmd(ADC1,ENABLE);
	
	//У׼�����Է�װ��������
	ADC_ResetCalibration(ADC1); 						//������λУ׼ 
	while(ADC_GetResetCalibrationStatus(ADC1));			//�ȴ���λУ׼����
	ADC_StartCalibration(ADC1); 						//���� AD У׼
	while(ADC_GetCalibrationStatus(ADC1)); 				//�ȴ�У׼����	
}

/*
��ȡADC��ֵ
ch��ͨ��ֵ��0~3
1�����ù���ͨ������
2���������ת��
3���ȴ�ת����ɣ���ȡADCֵ
*/
u16 Get_ADC(u8 ch)
{
	//����ָ�� ADC �Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC1,ch,0x1,ADC_SampleTime_239Cycles5);
	//ʹ�����ת������
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//�ȴ�ת������
	
	return ADC_GetConversionValue(ADC1);		//�������һ�� ADC1 �������ת�����
	
}

/*
��ε���Get_ADC()������ȡ֮��ȥ��ƽ��ֵ
*/
u16 Get_ADC_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{ 	
		temp_val+=Get_ADC(ch);
		delay_ms(5);
	}
	return temp_val/times;
}

