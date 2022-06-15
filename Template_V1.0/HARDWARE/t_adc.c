#include "t_adc.h"
#include "delay.h"


/*
1、初始化里面完成开启PA口时钟和ADC1时钟，设置PA1为模拟输入
2、复位ADC1，同时设置ADC1分频因子
3、初始化ADC1参数，设置ADC1的工作模式以及规则序列的相关信息
开启内部温度传感器
4、使能ADC并校准
*/
void T_ADC_Init(void)//ADC初始化 
{
	ADC_InitTypeDef ADC_InitStruct;
	//使能 GPIOA,ADC1 通道时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE );
	
	//设置ADC1的分频因子为6，因为ADC的最高频率不能超过14M，72M/6=12,
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	ADC_DeInit(ADC1);
	
	//初始化ADC1
	ADC_InitStruct.ADC_ContinuousConvMode =DISABLE;//模数转换工作在单次转换模式
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//ADC数据右对齐
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent; //ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStruct.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	
	ADC_Init(ADC1,&ADC_InitStruct);
	
	ADC_TempSensorVrefintCmd(ENABLE); //开启内部温度传感器
	ADC_Cmd(ADC1,ENABLE);				//使能指定的ADC1
	
	//校准（可以封装个函数）
	ADC_ResetCalibration(ADC1); 						//开启复位校准 
	while(ADC_GetResetCalibrationStatus(ADC1));			//等待复位校准结束
	ADC_StartCalibration(ADC1); 						//开启 AD 校准
	while(ADC_GetCalibrationStatus(ADC1)); 				//等待校准结束
	}

u16 T_Get_Adc(u8 ch)  
{
	//设置指定 ADC 的规则组通道，设置它们的转化顺序和采样时间
	ADC_RegularChannelConfig(ADC1,ch,0x1,ADC_SampleTime_239Cycles5);
	//使能软件转换功能
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//等待转换结束
	
	return ADC_GetConversionValue(ADC1);		//返回最近一次 ADC1 规则组的转换结果
}

//得到ADC采样内部温度传感器的值
//取10次,然后平均
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
	
 //获取通道ch的转换值
//取times次,然后平均
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

//得到温度值
//返回值:温度值(扩大了100倍,单位:℃.)
short Get_Temprate(void)	//获取内部温度传感器温度值
{
	u32 adcx;
	short result;
 	double temperate;
	adcx=T_Get_Adc_Average(ADC_Channel_16,20);	//读取通道16,20次取平均
	temperate=(float)adcx*(3.3/4096);		//电压值 
	temperate=(1.43-temperate)/0.0043+25;	//转换为温度值 	 
	result=temperate*=100;					//扩大100倍.
	return result;
}
