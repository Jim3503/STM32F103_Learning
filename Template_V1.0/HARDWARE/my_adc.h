#ifndef __MY_ADC_H
#define __MY_ADC_H
#include "sys.h"

void My_ADC_Init(void);//ADC��ʼ�� 
/*
1����ʼ��������ɿ���PA��ʱ�Ӻ�ADC1ʱ�ӣ�����PA1Ϊģ������
2����λADC1��ͬʱ����ADC1��Ƶ����
3����ʼ��ADC1����������ADC1�Ĺ���ģʽ�Լ��������е������Ϣ
4��ʹ��ADC��У׼
*/

u16 Get_ADC(u8 ch);
/*
1�����ù���ͨ������
2���������ת��
3���ȴ�ת����ɣ���ȡADCֵ
*/
u16 Get_ADC_Average(u8 ch,u8 times);
/*
��ε���Get_ADC()������ȡ֮��ȥ��ƽ��ֵ
*/
#endif
