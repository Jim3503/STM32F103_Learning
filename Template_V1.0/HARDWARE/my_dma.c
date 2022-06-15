#include"my_dma.h"

/*
通道配置过程
1. 在DMA_CPARx寄存器设置外设寄存器的地址，发生外设数据传输请求时候，这个地址将是数据传输的源或目
标
2.在DMA_CMARx寄存器中设置数据存储器的地址，发生外设数据传输请求时，传输的数据将从这个地址读出或写
入这个地址
3.在DMA_CNDTRx寄存器中设置要传输的数据量，在每个数据传输后，这个数字递减
4.在DMA_CCRx寄存器的PL[1:0]位中设置通道的优先级
5.在DMA_CCRx寄存器中设置数据传输的方向，循环模式，外设和存储器的增量模式、外设和存储器的数据宽度
、传输一般产生中断或传输完成产生中断
6.设置DMA_CCRx寄存器的ENABLE位，启动该通道
一旦启动了DMA通道，既可响应连到该通道上的外设的DMA请求
当传输一半的数据后，半传输标志（HTIF）被置为1，当设了允许半传输中断位（HTIE）时，将产生一个中断请
求，在数据传输结束后，传输完成标志（TCIF）被置为1，当设置了允许传输完成中断位（TCIE）时，将产生一
个中断请求。
*/


DMA_InitTypeDef DMA_InitStructure;
u16 DMA1_MEM_LEN;//保存 DMA 每次数据传送的长度 
//DMA1 的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8 位数据宽度/存储器增量模式
//DMA_CHx:DMA 通道 CHx
//cpar:外设地址
//cmar:存储器地址
//cndtr:数据传输量

/*
DMA配置的过程
1.使能DMA时钟
2.初始化DMA通道参数
3.使能串口DMA发送，串口DMA使能函数
4.使能DMA1通道，启动传输
5.查询DMA传输状态
6.获取/设置通道当前剩余数据量
*/

//配置DMA_CHx
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
	//1.使能DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);   //使能DMA时钟
	DMA_DeInit(DMA_CHx); 					//将 DMA 的通道 1 寄存器重设为缺省值
	DMA1_MEM_LEN = cndtr;					//DMA 每次数据传送的长度
	
	//2.初始化DMA通道参数
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar; //DMA 外设 ADC 基地址
	DMA_InitStructure.DMA_MemoryBaseAddr =  cmar; //DMA 内存基地址	
	
	DMA_InitStructure.DMA_DIR =  DMA_DIR_PeripheralDST; //数据传输方向内存到外设
	DMA_InitStructure.DMA_BufferSize = cndtr; //DMA 通道的 DMA 缓存的大小
	
	DMA_InitStructure.DMA_PeripheralInc =  DMA_PeripheralInc_Disable; //外设地址不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址寄存器递增
	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //数据宽度为 8 位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为 8 位
		
	DMA_InitStructure.DMA_Mode =  DMA_Mode_Normal; //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DM 通道拥有中优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //非内存到内存传输


	DMA_Init(DMA_CHx,&DMA_InitStructure);//初始化DMA的通道
}

//开启一次DMA传输和使能DMA
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{	
	DMA_Cmd(DMA_CHx, DISABLE ); //关闭 USART1 TX DMA1 所指示的通道
	DMA_SetCurrDataCounter(DMA1_Channel4,DMA1_MEM_LEN);//设置 DMA 缓存的大小而且这个缓存的大小是之前设定的cndtr
	DMA_Cmd(DMA_CHx, ENABLE); //使能 USART1 TX DMA1 所指示的通道
}
