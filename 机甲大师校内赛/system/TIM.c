#include "stm32f10x.h"                  // Device header

void myTIM_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	  
	TIM_InternalClockConfig(TIM6);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 2400-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1200-1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	
	NVICInitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVICInitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVICInitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVICInitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVICInitStructure);
	
	TIM_Cmd(TIM6, ENABLE);
}

