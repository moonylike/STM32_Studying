#include "stm32f10x.h"                  // Device header

//按下Key1后，关闭LED1
void EXTI_NVIC_Init (void){
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_AFIO, ENABLE);		//开时钟
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);	//EXTI输入
	
	EXTI_InitTypeDef EXTI_InitStructure;						//EXTI输出
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//下降沿触发
	EXTI_Init (&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2);			//配置NVIC
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//响应优先级
	NVIC_Init (&NVIC_InitStructure);
}
void EXTI0_IRQHandler (void){
	if (EXTI_GetITStatus(EXTI_Line0) == SET){
		GPIO_SetBits (GPIOB, GPIO_Pin_0);		//灭灯
		EXTI_ClearITPendingBit(EXTI_Line0);		//清除寄存器
	}
}
