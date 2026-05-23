#include "stm32f10x.h"                  // Device header
#include "LED.h"


extern uint8_t LEDstate;

void TIM2_Init(void){
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrucrure;
	TIM_TimeBaseStructInit (&TIM_TimeBaseInitStrucrure);
	TIM_TimeBaseInitStrucrure.TIM_Period = 5000 - 1;	//自动重装
	TIM_TimeBaseInitStrucrure.TIM_Prescaler	= 7200 - 1;	//预分频
	TIM_TimeBaseInit (TIM2,&TIM_TimeBaseInitStrucrure);
	
	TIM_Cmd(TIM2, ENABLE);
	
	TIM_ITConfig (TIM2, TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2);	//分组
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	//选择中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
}
 void TIM2_IRQHandler (void){
	if (TIM_GetITStatus (TIM2, TIM_IT_Update) == SET){
		if (LEDstate){
			LED1_Turn();
		}
		TIM_ClearITPendingBit (TIM2, TIM_IT_Update);
	}
 }
