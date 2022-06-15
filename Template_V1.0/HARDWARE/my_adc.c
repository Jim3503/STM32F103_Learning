#include "my_adc.h"
#include "delay.h"
/*
1、初始化里面完成开启PA口时钟和ADC1时钟，同时对GPIO进行初始化设置PA1为模拟输入
2、复位ADC1，同时设置ADC1分频因子
3、初始化ADC1参数，设置ADC1的工作模式以及规则序列的相关信息
4、使能ADC并校准
*/
void My_ADC_Init(void)//ADC初始化 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef  ADC_InitStruct;
	//开启PA口时钟和ADC1时钟，
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
	//初始化GPIOA，设置PA1为模拟输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //-->PA.1 模拟输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //模拟输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//设置ADC1的分频因子为6，因为ADC的最高频率不能超过14M，72M/6=12,
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	ADC_DeInit(ADC1);
	
	//初始化ADC1
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;		//单次转换模式
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;		//ADC 数据右对齐
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//转换由软件而不是外部
																	//触发启动
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;					//ADC 独立模式
	ADC_InitStruct.ADC_NbrOfChannel = 0x1;				//顺序进行规则转换的ADC 通道的数目
	ADC_InitStruct.ADC_ScanConvMode = DISABLE; 			//单通道模式
	
	ADC_Init(ADC1,&ADC_InitStruct);
	
	//使能ADC1
	ADC_Cmd(ADC1,ENABLE);
	
	//校准（可以封装个函数）
	ADC_ResetCalibration(ADC1); 						//开启复位校准 
	while(ADC_GetResetCalibrationStatus(ADC1));			//等待复位校准结束
	ADC_StartCalibration(ADC1); 						//开启 AD 校准
	while(ADC_GetCalibrationStatus(ADC1)); 				//等待校准结束	
}

/*
获取ADC的值
ch是通道值，0~3
1、配置规则通道参数
2、开启软件转换
3、等待转换完成，读取ADC值
*/
u16 Get_ADC(u8 ch)
{
	//设置指定 ADC 的规则组通道，设置它们的转化顺序和采样时间
	ADC_RegularChannelConfig(ADC1,ch,0x1,ADC_SampleTime_239Cycles5);
	//使能软件转换功能
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//等待转换结束
	
	return ADC_GetConversionValue(ADC1);		//返回最近一次 ADC1 规则组的转换结果
	
}

/*
多次调用Get_ADC()函数读取之后去得平均值
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

