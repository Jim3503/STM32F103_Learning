#ifndef __LIGHT_ADC_H
#define __LIGHT_ADC_H
#include "sys.h"

void Light_ADC_Init(void);//ADC��ʼ�� 
/*
1����ʼ��������ɿ���PA��ʱ�Ӻ�ADC1ʱ�ӣ�����PA1Ϊģ������
2����λADC1��ͬʱ����ADC1��Ƶ����
3����ʼ��ADC1����������ADC1�Ĺ���ģʽ�Լ��������е������Ϣ
4��ʹ��ADC��У׼
*/
void Adc3_Init(void);

u16 Get_Adc3(u8 ch);
/*
1�����ù���ͨ������
2���������ת��
3���ȴ�ת����ɣ���ȡADCֵ
*/
u8 Lsens_Get_Val(void);
/*
��ε���Get_Adc3()������ȡ֮��ȥ��ƽ��ֵ
*/

 
#define LSENS_READ_TIMES	10		//���������������ȡ����,����ô���,Ȼ��ȡƽ��ֵ
#define LSENS_ADC_CHX		ADC_Channel_6	//����������������ڵ�ADCͨ�����
#endif
