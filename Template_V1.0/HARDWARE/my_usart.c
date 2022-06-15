#include "my_usart.h"
#include "my_adc.h"


/*浮空输入_IN_FLOATING
带上拉输入_IPU
带下拉输入_IPD
模拟输入_AIN
开漏输出_OUT_OD
推挽输出_OUT_PP
开漏复用输出_AF_OD
推挽复用输出_AF_PP*/

void My_Usart_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	//串口时钟和复用GPIO使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE); //使能 PA,USART1 端口时钟
	//PA9,IO初始化
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//PA10，IO初始化
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//串口参数初始化
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART1,&USART_InitStruct);
	//串口使能
	USART_Cmd(USART1,ENABLE);
	
	//配置完这些需要开启串口响应中断，此时先在main函数里面添加NVIC_PriorityGroupConfig(NVIC_PriorityGroup_x);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);			//打开接收中断
	
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;			//定义初始化的是哪个中断，这个我们可以在 stm32f10x.h 中找到
															//每个中断对应的名字。例如 USART1_IRQn。
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;			//该中断是否使能。
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority =1;   //定义这个中断的抢占优先级别。
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;		    //定义这个中断的子优先级别。
	NVIC_Init(&NVIC_InitStruct);
}

//配置完中断之后开始写中断服务函数这个是在启动文件里面的，每次接收到数据就开始执行中断服务函数
//获取相应的中断状态，当该中断发生了就会设置状态寄存器中的某个标志位，
//经常在中断服务函数里面，要判断该中断是哪种中断，使用的函数是相应外设的GetITStatus里面
//USART_ReceiveData()函数到stm32f10x_usart.h文件里面寻找
////
//void USART1_IRQHandler(void)
//{
//	u8 res;
//	if(USART_GetITStatus(USART1,USART_IT_RXNE))
//	{
//		res = USART_ReceiveData(USART1);
//		USART_SendData(USART1,res);
//	}
//}

