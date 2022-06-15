#ifndef __LIGHT_ADC_H
#define __LIGHT_ADC_H
#include "sys.h"

void Light_ADC_Init(void);//ADC初始化 
/*
1、初始化里面完成开启PA口时钟和ADC1时钟，设置PA1为模拟输入
2、复位ADC1，同时设置ADC1分频因子
3、初始化ADC1参数，设置ADC1的工作模式以及规则序列的相关信息
4、使能ADC并校准
*/
void Adc3_Init(void);

u16 Get_Adc3(u8 ch);
/*
1、配置规则通道参数
2、开启软件转换
3、等待转换完成，读取ADC值
*/
u8 Lsens_Get_Val(void);
/*
多次调用Get_Adc3()函数读取之后去得平均值
*/

 
#define LSENS_READ_TIMES	10		//定义光敏传感器读取次数,读这么多次,然后取平均值
#define LSENS_ADC_CHX		ADC_Channel_6	//定义光敏传感器所在的ADC通道编号
#endif
