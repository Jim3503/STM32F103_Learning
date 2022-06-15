#include "key.h"
#include "sys.h" 
#include "delay.h" 
//按键初始化函数
void KEY_Init(void) //IO 初始化
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|
		RCC_APB2Periph_GPIOE,ENABLE); 						//使能 PORTA,PORTE 时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;	//GPIOE.3~4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 			//设置成上拉输入
	GPIO_Init(GPIOE, &GPIO_InitStructure);					 //初始化 GPIOE3,4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		 		//初始化 WK_UP-->GPIOA.0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		//PA0 设置成输入，下拉 
	GPIO_Init(GPIOA, &GPIO_InitStructure); 		//初始化 GPIOA.0
}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0 按下
//2，KEY1 按下
//3，KEY3 按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY_UP!!
u8 KEY_Scan(u8 mode)
{
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1; //支持连按 
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
	{
		delay_ms(10);//去抖动
		key_up=0;
	if(KEY0==0)return KEY0_PRES;
	else if(KEY1==0)return KEY1_PRES;
	else if(WK_UP==1)return WKUP_PRES;
	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1; 
	return 0;// 无按键按下
}
