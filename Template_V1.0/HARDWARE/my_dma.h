#ifndef __MY_DMA_H
#define __MY_DMA_H
#include "sys.h"

void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);//DMA����
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);			//DMAʹ�ܣ�����һ��DMA����


#endif
