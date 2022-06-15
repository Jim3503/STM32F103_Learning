#include "exti.h"
#include "delay.h"
#include "key.h"
#include "led.h"
/*注意此处代码是学习key0h和key1的配置外部中断的原理*/
void EXITX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	//初始化GPIO口为输入
	//这边可以添加按键的初始化，这边尚未编写并添加key.c文件key.Init()里面已经有GPIO的初始化配置
	//key_Init()
	
	//开启IO复用时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//设置IO口和中断线的映射关系，key0和key1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
	//初始化线上中断，设置触发条件
	EXTI_InitStruct.EXTI_Line = EXTI_Line4;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line3;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
}
//编写中断服务函数，中断服务函数的格式在启动文件里面
void EXTI4_IRQHandler(void)
{
	delay_ms(10);
	if(KEY0 == 0)
	{
		LED0 = !LED0;
		LED1 = !LED1;
	}
	
	//清除中断标志位
	EXTI_ClearITPendingBit(EXTI_Line4);
}
//编写中断服务函数，中断服务函数在启动文件里面
void EXTI3_IRQHandler(void)
{
	delay_ms(10);
	if(KEY1 == 0)
	{
		LED0 = !LED0;
		LED1 = !LED1;
	}
	
	//清除中断标志位
	EXTI_ClearITPendingBit(EXTI_Line3);
}

