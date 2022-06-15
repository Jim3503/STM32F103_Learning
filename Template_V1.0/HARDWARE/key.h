#ifndef __KEY_H
#define __KEY_H
#include "sys.h"
#define KEY0 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) // 读取按键0
#define KEY1 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) // 读取按键1 
#define WK_UP GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) // 读取按键WK_UP
/**************************位操作*****************************************
#define KEY0 PEin(4) //PE4
#define KEY1 PEin(3) //PE3 
#define WK_UP PAin(0) //PA0 WK_UP

***************************位操作*************************************************/

#define KEY0_PRES 1 //KEY0 按下
#define KEY1_PRES 2 //KEY1 按下
#define WKUP_PRES 3 //WK_UP 按下

void KEY_Init(void); //IO 初始化
u8 KEY_Scan(u8); //按键扫描函数
#endif
