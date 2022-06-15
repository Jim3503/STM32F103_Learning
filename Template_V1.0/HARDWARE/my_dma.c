#include"my_dma.h"

/*
ͨ�����ù���
1. ��DMA_CPARx�Ĵ�����������Ĵ����ĵ�ַ�������������ݴ�������ʱ�������ַ�������ݴ����Դ��Ŀ
��
2.��DMA_CMARx�Ĵ������������ݴ洢���ĵ�ַ�������������ݴ�������ʱ����������ݽ��������ַ������д
�������ַ
3.��DMA_CNDTRx�Ĵ���������Ҫ���������������ÿ�����ݴ����������ֵݼ�
4.��DMA_CCRx�Ĵ�����PL[1:0]λ������ͨ�������ȼ�
5.��DMA_CCRx�Ĵ������������ݴ���ķ���ѭ��ģʽ������ʹ洢��������ģʽ������ʹ洢�������ݿ��
������һ������жϻ�����ɲ����ж�
6.����DMA_CCRx�Ĵ�����ENABLEλ��������ͨ��
һ��������DMAͨ�����ȿ���Ӧ������ͨ���ϵ������DMA����
������һ������ݺ󣬰봫���־��HTIF������Ϊ1������������봫���ж�λ��HTIE��ʱ��������һ���ж���
�������ݴ�������󣬴�����ɱ�־��TCIF������Ϊ1��������������������ж�λ��TCIE��ʱ��������һ
���ж�����
*/


DMA_InitTypeDef DMA_InitStructure;
u16 DMA1_MEM_LEN;//���� DMA ÿ�����ݴ��͵ĳ��� 
//DMA1 �ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8 λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMA ͨ�� CHx
//cpar:�����ַ
//cmar:�洢����ַ
//cndtr:���ݴ�����

/*
DMA���õĹ���
1.ʹ��DMAʱ��
2.��ʼ��DMAͨ������
3.ʹ�ܴ���DMA���ͣ�����DMAʹ�ܺ���
4.ʹ��DMA1ͨ������������
5.��ѯDMA����״̬
6.��ȡ/����ͨ����ǰʣ��������
*/

//����DMA_CHx
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
	//1.ʹ��DMAʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);   //ʹ��DMAʱ��
	DMA_DeInit(DMA_CHx); 					//�� DMA ��ͨ�� 1 �Ĵ�������Ϊȱʡֵ
	DMA1_MEM_LEN = cndtr;					//DMA ÿ�����ݴ��͵ĳ���
	
	//2.��ʼ��DMAͨ������
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar; //DMA ���� ADC ����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr =  cmar; //DMA �ڴ����ַ	
	
	DMA_InitStructure.DMA_DIR =  DMA_DIR_PeripheralDST; //���ݴ��䷽���ڴ浽����
	DMA_InitStructure.DMA_BufferSize = cndtr; //DMA ͨ���� DMA ����Ĵ�С
	
	DMA_InitStructure.DMA_PeripheralInc =  DMA_PeripheralInc_Disable; //�����ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ��ַ�Ĵ�������
	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //���ݿ��Ϊ 8 λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ 8 λ
		
	DMA_InitStructure.DMA_Mode =  DMA_Mode_Normal; //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DM ͨ��ӵ�������ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //���ڴ浽�ڴ洫��


	DMA_Init(DMA_CHx,&DMA_InitStructure);//��ʼ��DMA��ͨ��
}

//����һ��DMA�����ʹ��DMA
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{	
	DMA_Cmd(DMA_CHx, DISABLE ); //�ر� USART1 TX DMA1 ��ָʾ��ͨ��
	DMA_SetCurrDataCounter(DMA1_Channel4,DMA1_MEM_LEN);//���� DMA ����Ĵ�С�����������Ĵ�С��֮ǰ�趨��cndtr
	DMA_Cmd(DMA_CHx, ENABLE); //ʹ�� USART1 TX DMA1 ��ָʾ��ͨ��
}
