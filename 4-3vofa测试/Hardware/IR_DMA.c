#include "stm32f10x.h"                  // Device header
#include "USART.h"

//定义数组用来存放模拟值
uint16_t Value[4] = {0};

void IRDMAInit (void){
	//ADC触发
	ADC_DMACmd (ADC1, ENABLE);
	
	RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_InitTypeDef DMA_InitStructure;
	//外设站点
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;				//外设站点基地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//传输数据长度
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//外设站点地址是否自增
	//存储器站点
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Value;						//存储器站点基地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//传输数据长度
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//存储器站点地址是否自增
	//其他设置
	DMA_InitStructure.DMA_BufferSize = 4;										//传输次数
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//关闭循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;							//优先级
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;							//传输方向
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								//软件触发？

	DMA_Init (DMA1_Channel1, &DMA_InitStructure);
	
	DMA_Cmd (DMA1_Channel1, ENABLE);
	
//	DMA_ITConfig (DMA1_Channel1, DMA_IT_TC, ENABLE);
}

//void DMA1_Channel1_IRQHandler(void){
//	
//	DMA_ClearFlag (DMA1_FLAG_TC1);
//}

//void IRDMA_Transfer (void){
//	
//	DMA_Cmd (DMA1_Channel1, DISABLE);					//关闭DMA（关闭后才能写入计数器）
//	DMA_SetCurrDataCounter (DMA1_Channel1, 4);			//对DMA的计数器写入4
//	DMA_Cmd (DMA1_Channel1, ENABLE);					//开启DMA

//	ADC_SoftwareStartConvCmd (ADC1, ENABLE);			//软件触发ADC
//	
//	while (DMA_GetFlagStatus (DMA1_FLAG_TC1) ==RESET);	//等待DMA转换完成
//	DMA_ClearFlag (DMA1_FLAG_TC1);						//清除标志位
//}
