#include "stm32f10x.h"                  // Device header
#include "USART.h"

/*
ADC123	IN11	IN12	IN13
模拟输入PC1		PC2		PC3

ADC12	IN14	IN15
模拟输入PC4		PC5

ADC3	IN4	IN5	IN6	IN7	IN8
模拟输入PF6	PF7	PF8	PF9	PF10
*/

void IRADCInit (void){
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);
	
	RCC_ADCCLKConfig (RCC_PCLK2_Div6);		//6分频，因为主频是72M，所以分频后是12M
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AIN;		//模拟输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
//规则组填充通道，初始化ADC1， 通道0， 第一“菜单”， 采样时间55.5个采样周期
	ADC_RegularChannelConfig (ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig (ADC1, ADC_Channel_12, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig (ADC1, ADC_Channel_14, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig (ADC1, ADC_Channel_15, 4, ADC_SampleTime_55Cycles5);
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					//是否连续模式？
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				//右对齐
	ADC_InitStructure.ADC_ExternalTrigConv =ADC_ExternalTrigConv_None;	//外部触发选择
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					//独立工作模式
	ADC_InitStructure.ADC_NbrOfChannel = 4;								//通道有效数目
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;						//是否扫描模式？
	ADC_Init (ADC1, &ADC_InitStructure);

	ADC_Cmd (ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);							//复位校准
	while (ADC_GetResetCalibrationStatus(ADC1) ==SET);	//等待复位校准完成
	ADC_StartCalibration(ADC1);							//开始校准
	while (ADC_GetCalibrationStatus(ADC1) == SET);	//等待校准完成

}
