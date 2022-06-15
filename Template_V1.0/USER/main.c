#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "buzzer.h"
#include "my_usart.h"
#include "key.h"
#include "exti.h"
#include "my_adc.h"
#include "usart.h"
#include "t_adc.h"
#include "light_adc.h"
#include "my_dma.h"
#include "my_iic.h"
#include "24cxx.h"
#include "my_can.h"

u16 temp;
u16 temp_light;

#define SEND_BUF_SIZE 8200
u8 SendBuff[SEND_BUF_SIZE]; //发送数据缓冲区
const u8 TEXT_TO_SEND[]={"ALIENTEK ELITE STM32F1 DMA 串口实验"};

//要写入到24c02的字符串数组
const u8 TEXT_Buffer[]={"Elite STM32 IIC TEST"};
#define SIZE sizeof(TEXT_Buffer)	

int main(void)
{
	u16 i;
	u8 j,mask=0;
	u8 t=0;
	float pro=0;//进度
	u8 datatemp[SIZE];
	u8 canbuf[8] = {'1','2','3','4','5','6','\r','\n'};		//定义一个CAN收发数据的缓冲区
	u8 mode = CAN_Mode_LoopBack;	//CAN工作模式;CAN_Mode_Normal(0)：普通模式，
									//CAN_Mode_LoopBack(1)：环回模式
	u8 res;
	u8 canbuf1[8];
	u8 temp;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
	//LED_Init();
	//My_Usart_Init();
	//Buzzer_Init();
	KEY_Init();
	EXITX_Init();
	//My_ADC_Init();
	uart_init(115200);
	T_ADC_Init();
	Light_ADC_Init();
	AT24CXX_Init(); //IIC 初始化
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);//CAN初始化环回模式,波特率500Kbps    

/********************************DMA串口实验******************************************	
	MYDMA_Config(DMA1_Channel4,(u32)&USART1->DR,(u32)SendBuff,SEND_BUF_SIZE);
	
	j=sizeof(TEXT_TO_SEND); 
		for(i=0;i<SEND_BUF_SIZE;i++)//填充数据到 SendBuff
		{
			if(t>=j)//加入换行符
			{
				if(mask)
				{
				SendBuff[i]=0x0a;
				t=0;
				}else
				{
					SendBuff[i]=0x0d;
					mask++;
				}
			}else//复制 TEXT_TO_SEND 语句
			{
				mask=0;
				SendBuff[i]=TEXT_TO_SEND[t];
				t++;
			}
		}
*/
//******************************************************************
	 while(1)
		{
//*******************GPIO_SetBits和GPIO_ResetBits的使用***********************************************
//			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
//			GPIO_SetBits(GPIOE,GPIO_Pin_5);
//			delay_ms(300);
//			//Delay(3000000);
//			GPIO_SetBits(GPIOB,GPIO_Pin_5);
//			GPIO_ResetBits(GPIOE,GPIO_Pin_5);
//			delay_ms(300);
//			//Delay(3000000);
//*******************GPIO_SetBits和GPIO_ResetBits的使用***********************************************

//*******************跑马灯led.h里面的那个#define LED0 PBout(5)// DS0的通过位带操作对IO的控制***********************************************
//			LED0=0; 
//			LED1=1;
//			delay_ms(1000); //延时 300ms
//			LED0=1;
//			LED1=0;
//			delay_ms(1000); //延时 300ms
//*******************led.h里面的那个#define LED0 PBout(5)// DS0的通过位带操作对IO的控制***********************************************
//操作IO之前先得对时钟使能，再对IO进行初始化，可以通过库函数也可以通过位操作或者寄存器来进行控制IO
/********************蜂鸣器的使用**************************************************
//			Buzzer0 = 1;
//			delay_ms(1000); //延时 300ms
//			Buzzer0 = 0;
//			delay_ms(1000); //延时 300ms
////			GPIO_ResetBits(GPIOB,GPIO_Pin_8);
////			delay_ms(500);
////			GPIO_SetBits(GPIOB,GPIO_Pin_8);
////			delay_ms(500);
**************************************************************************/

/*************************读取ADC1(PA1)转换的值，并且利用串口printf进行输出*******/
//	temp = Get_ADC(1);
//	temp=(float)temp*(3.3/4096);
//	printf( "采集到的电压值：%d V \n",temp);
/******************************************************************************/

/******************************采集内部温度*****************************************/
		temp = Get_Temprate();
		printf("内部温度：%d \r\n",temp/100);
/******************************采集内部温度*****************************************/

/******************************采集光线强度*****************************************/
		temp_light = Lsens_Get_Val();
		printf("光线强度: %d \r\n",temp_light);
/******************************采集光线强度*****************************************/
/*****************************DMA串口实验*****************************************/
/*
USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
		//使能串口 1 的 DMA 发送
		MYDMA_Enable(DMA1_Channel4);//开始一次 DMA 传输！
		
		while(1)
		{
			if(DMA_GetFlagStatus(DMA1_FLAG_TC4)!=RESET)//判断通道 4 传输完成
			{
				DMA_ClearFlag(DMA1_FLAG_TC4);//清除通道 4 传输完成标志
				break; 
			}
			pro=DMA_GetCurrDataCounter(DMA1_Channel4);//得到当前剩余数据量
			pro=1-pro/SEND_BUF_SIZE;//得到百分比
			pro*=100; //扩大 100 倍
			printf("传输进度：%f",pro);
		}
*/		
/******************************DMA串口实验*****************************************/

/***********************************IIC实验****************************************/
	while(AT24CXX_Check())//检测不到24c02
	{
		LED0=!LED0;//DS0闪烁
	}
	AT24CXX_Write(0,(u8*)TEXT_Buffer,SIZE);
	AT24CXX_Read(0,datatemp,SIZE);
	//printf("%s",datatemp);
/***********************************IIC实验****************************************/
	
/***********************************CAN实验****************************************/
	res=Can_Send_Msg(canbuf,8);//发送8个字节 ,发送成功是1，发送失败是0
	if(res) printf("发送成功\r\n");
	temp = Can_Receive_Msg(canbuf1);
	if(temp)
	{
		printf("接收到数据:%s",canbuf1);
	}


}
}
