#include "stm32f10x.h"                  // Device header

uint16_t AD_Status;
uint16_t AD_Value[4];
uint16_t AD_Flag;
uint16_t i=0;

uint16_t ADC_Data[4];

void ADCDetect_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1| GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 4, ADC_SampleTime_55Cycles5);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_NbrOfChannel = 4;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	//选择外设初始地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	//一个数据搬运16位
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//地址不自增
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_Data;
	//选择存储器初始地址，我们要把数据搬运到数据包里
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	//一个数据搬运16位
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//地址自增
	//我们这里对自增的配置就可以使得ADC1的DR寄存器的数据不断地进入DMA了
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	//DMA设置成循环模式
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	//我们这里要使用外设告诉他什么时候进行搬运，所以M2M置为0
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	//这里是配置方向，我们从外设方向向存储器方向搬运数据
	DMA_InitStructure.DMA_BufferSize = 4;
	//一次搬运四个
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	//这里的优先级随便配置一个就行，这里只有一个DMA通道要进行搬运数据
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	DMA_Cmd(DMA1_Channel1, ENABLE);//DMA上电
	ADC_DMACmd(ADC1, ENABLE);//这样就可以通过ADC1触发DMA的工作了
	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) == SET);
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

uint32_t forward1_GetValue(void)
{
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9) == 1) return 1;
	return 0;
}

uint32_t forward2_GetValue(void)
{
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11) == 1) return 1;
	return 0;
}

uint32_t forward3_GetValue(void)
{
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13) == 1) return 1;
	return 0;
}

uint32_t forward4_GetValue(void)
{
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_14) == 1) return 1;
	return 0;
}

uint32_t forward5_GetValue(void)
{
	if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12) == 1) return 1;
	return 0;
}

uint32_t forward6_GetValue(void)
{
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 1) return 1;
	return 0;
}

uint32_t forward7_GetValue(void)
{
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 1) return 1;
	return 0;
}

uint32_t forward8_GetValue(void)
{
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == 1) return 1;
	return 0;
}

uint32_t backLeft_GetValue(void)
{
	return ADC_Data[0];
}

uint32_t middleLeft_GetValue(void)
{
	return ADC_Data[1];
}

uint32_t middleRight_GetValue(void)
{
	return ADC_Data[2];
}

uint32_t backRight_GetValue(void)
{
	return ADC_Data[3];
}

void AD_Detect(uint16_t ADCx, uint8_t ADC_Channel)
{
	if(ADCx == 1)
	{
		ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
		//选择哪一个通道
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		//开始转换
		while(!AD_Flag);//如果仍然没有转换结束，我们就留在这里，防止出错
		AD_Flag=0;
	}
}

void ADC1_2_IRQHandler(void)
{
	if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == SET)
	{
		AD_Flag=1;
		ADC_Data[i++] = ADC_GetConversionValue(ADC1);
		if(i==9) i=0;
		ADC_ClearITPendingBit(ADC1, ADC_FLAG_EOC);
	}
}
