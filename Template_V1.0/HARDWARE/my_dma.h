#ifndef __MY_DMA_H
#define __MY_DMA_H
#include "sys.h"

void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);//DMA配置
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);			//DMA使能，开启一次DMA传输


#endif
