#ifndef __MY_ADC_H
#define __MY_ADC_H
#include "sys.h"

void My_ADC_Init(void);//ADC初始化 
/*
1、初始化里面完成开启PA口时钟和ADC1时钟，设置PA1为模拟输入
2、复位ADC1，同时设置ADC1分频因子
3、初始化ADC1参数，设置ADC1的工作模式以及规则序列的相关信息
4、使能ADC并校准
*/

u16 Get_ADC(u8 ch);
/*
1、配置规则通道参数
2、开启软件转换
3、等待转换完成，读取ADC值
*/
u16 Get_ADC_Average(u8 ch,u8 times);
/*
多次调用Get_ADC()函数读取之后去得平均值
*/
#endif
