#include "t_adc.h"
#include "delay.h"


/*
1����ʼ��������ɿ���PA��ʱ�Ӻ�ADC1ʱ�ӣ�����PA1Ϊģ������
2����λADC1��ͬʱ����ADC1��Ƶ����
3����ʼ��ADC1����������ADC1�Ĺ���ģʽ�Լ��������е������Ϣ
�����ڲ��¶ȴ�����
4��ʹ��ADC��У׼
*/
void T_ADC_Init(void)//ADC��ʼ�� 
{
	ADC_InitTypeDef ADC_InitStruct;
	//ʹ�� GPIOA,ADC1 ͨ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE );
	
	//����ADC1�ķ�Ƶ����Ϊ6����ΪADC�����Ƶ�ʲ��ܳ���14M��72M/6=12,
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	ADC_DeInit(ADC1);
	
	//��ʼ��ADC1
	ADC_InitStruct.ADC_ContinuousConvMode =DISABLE;//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//ADC�����Ҷ���
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent; //ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStruct.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	
	ADC_Init(ADC1,&ADC_InitStruct);
	
	ADC_TempSensorVrefintCmd(ENABLE); //�����ڲ��¶ȴ�����
	ADC_Cmd(ADC1,ENABLE);				//ʹ��ָ����ADC1
	
	//У׼�����Է�װ��������
	ADC_ResetCalibration(ADC1); 						//������λУ׼ 
	while(ADC_GetResetCalibrationStatus(ADC1));			//�ȴ���λУ׼����
	ADC_StartCalibration(ADC1); 						//���� AD У׼
	while(ADC_GetCalibrationStatus(ADC1)); 				//�ȴ�У׼����
	}

u16 T_Get_Adc(u8 ch)  
{
	//����ָ�� ADC �Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC1,ch,0x1,ADC_SampleTime_239Cycles5);
	//ʹ�����ת������
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//�ȴ�ת������
	
	return ADC_GetConversionValue(ADC1);		//�������һ�� ADC1 �������ת�����
}

//�õ�ADC�����ڲ��¶ȴ�������ֵ
//ȡ10��,Ȼ��ƽ��
u16 T_Get_Temp(void)
{
	u16 temp_val=0;
	u8 t;
	for(t=0;t<10;t++)
	{
		temp_val+=T_Get_Adc(ADC_Channel_16);	  //TampSensor
		delay_ms(5);
	}
	return temp_val/10;
}
	
 //��ȡͨ��ch��ת��ֵ
//ȡtimes��,Ȼ��ƽ��
u16 T_Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=T_Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��,��λ:��.)
short Get_Temprate(void)	//��ȡ�ڲ��¶ȴ������¶�ֵ
{
	u32 adcx;
	short result;
 	double temperate;
	adcx=T_Get_Adc_Average(ADC_Channel_16,20);	//��ȡͨ��16,20��ȡƽ��
	temperate=(float)adcx*(3.3/4096);		//��ѹֵ 
	temperate=(1.43-temperate)/0.0043+25;	//ת��Ϊ�¶�ֵ 	 
	result=temperate*=100;					//����100��.
	return result;
}
