#ifndef __T_ADC_H
#define __T_ADC_H
#include "sys.h"

void T_ADC_Init(void);//ADC��ʼ�� 
/*
1����ʼ��������ɿ���PA��ʱ�Ӻ�ADC1ʱ�ӣ�����PA1Ϊģ������
2����λADC1��ͬʱ����ADC1��Ƶ����
3����ʼ��ADC1����������ADC1�Ĺ���ģʽ�Լ��������е������Ϣ
4��ʹ��ADC��У׼
*/

u16 T_Get_Adc(u8 ch);
/*
1�����ù���ͨ������
2���������ת��
3���ȴ�ת����ɣ���ȡADCֵ
*/
u16 T_Get_Temp(void);
u16 T_Get_Adc_Average(u8 ch,u8 times);
/*
��ε���Get_ADC()������ȡ֮��ȥ��ƽ��ֵ
*/
short Get_Temprate(void);
#endif
