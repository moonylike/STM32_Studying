#include "stm32f10x.h"                  // Device header

/*
ADC123	IN11	IN12
模拟输入PC1		PC2

ADC12	IN14	IN15
模拟输入PC4		PC5

ADC3	IN4	IN5	IN6	IN7	IN8
复用模式PF6	PF7	PF8	PF9	PF10
*/

void IRADCInit (void){
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_ADC3 | RCC_APB2Periph_GPIOF, ENABLE);
	
	RCC_ADCCLKConfig (RCC_PCLK2_Div6);		//6分频，因为主频是72M，所以分频后是12M
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AIN;		//模拟输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
//规则组填充通道，初始化ADC3， 通道0， 第一“菜单”， 采样时间55.5个采样周期
	ADC_RegularChannelConfig (ADC3, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					//是否连续模式？
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				//右对齐
	ADC_InitStructure.ADC_ExternalTrigConv =ADC_ExternalTrigConv_None;	//外部触发选择
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					//独立工作模式
	ADC_InitStructure.ADC_NbrOfChannel = 1;								//通道有效数目
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;						//是否扫描模式？
	ADC_Init (ADC3, &ADC_InitStructure);
	
	ADC_Cmd (ADC3, ENABLE);
	
	ADC_ResetCalibration(ADC3);							//复位校准
	while (ADC_GetResetCalibrationStatus(ADC3) ==SET);	//等待复位校准完成
	ADC_StartCalibration(ADC3);							//开始校准
	while (ADC_GetCalibrationStatus(ADC3) == SET);		//等待校准完成
	
}

//定义数组用来存放模拟值
uint16_t Value[4] = {0};

//使用函数给数组赋值
void Getvalue (void){
	ADC_RegularChannelConfig (ADC3, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);//通道4
	ADC_SoftwareStartConvCmd (ADC3, ENABLE);									//软件触发转换
	while (ADC_GetFlagStatus (ADC3, ADC_FLAG_EOC) == RESET);					//等待转换完成
	Value[0] = ADC_GetConversionValue (ADC3);									//获取转换值，并清除标志位
	
	ADC_RegularChannelConfig (ADC3, ADC_Channel_6, 1, ADC_SampleTime_55Cycles5);//通道6
	ADC_SoftwareStartConvCmd (ADC3, ENABLE);
	while (ADC_GetFlagStatus (ADC3, ADC_FLAG_EOC) == RESET);
	Value[1] = ADC_GetConversionValue (ADC3);
	
	ADC_RegularChannelConfig (ADC3, ADC_Channel_7, 1, ADC_SampleTime_55Cycles5);//通道7
	ADC_SoftwareStartConvCmd (ADC3, ENABLE);
	while (ADC_GetFlagStatus (ADC3, ADC_FLAG_EOC) == RESET);
	Value[2] = ADC_GetConversionValue (ADC3);
	
	ADC_RegularChannelConfig (ADC3, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);//通道8
	ADC_SoftwareStartConvCmd (ADC3, ENABLE);
	while (ADC_GetFlagStatus (ADC3, ADC_FLAG_EOC) == RESET);
	Value[3] = ADC_GetConversionValue (ADC3);
}
