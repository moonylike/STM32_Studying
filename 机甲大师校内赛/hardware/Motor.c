#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "PID.h"
#include "TIM.h"

void Motor_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	GPIO_InitTypeDef GPIOInitStructure;//定义结构体
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//开启AFIO的时钟,为引脚重映射做准备
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIOInitStructure);
	
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_3| GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIOInitStructure);
	
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIOInitStructure);
	
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIOInitStructure);
	
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIOInitStructure);
    
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 20000-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    //下面是OC(输出比较模块)的配置
	TIM_OCStructInit(&TIM_OCInitStructure);//首先置为初始值，防止由于其为局部变量而造成内部成员值的不确定
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//选择PWM1模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//这里极性不进行翻转
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//使能
	TIM_OCInitStructure.TIM_Pulse = 6000;//这里选择的就是CCR的值
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
	
}

void SetRightSpeed(int32_t ccr)
{
	if(ccr>=0)
	{
		GPIO_ResetBits(GPIOG, GPIO_Pin_5);
		GPIO_SetBits(GPIOG, GPIO_Pin_4);
		GPIO_ResetBits(GPIOG, GPIO_Pin_7);
		GPIO_SetBits(GPIOG, GPIO_Pin_6);
		TIM_SetCompare1(TIM2, ccr);
		TIM_SetCompare2(TIM2, ccr);
	}
	else
	{
		GPIO_SetBits(GPIOG, GPIO_Pin_5);
		GPIO_ResetBits(GPIOG, GPIO_Pin_4);
		GPIO_SetBits(GPIOG, GPIO_Pin_7);
		GPIO_ResetBits(GPIOG, GPIO_Pin_6);
		TIM_SetCompare1(TIM2, 18000);
		TIM_SetCompare2(TIM2, -ccr);
	}
}

void SetLeftSpeed(int32_t ccr)
{
	if(ccr>=0)
	{
		GPIO_ResetBits(GPIOD, GPIO_Pin_13);
		GPIO_SetBits(GPIOC, GPIO_Pin_7);
		GPIO_ResetBits(GPIOD, GPIO_Pin_15);
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
		TIM_SetCompare3(TIM2, ccr);
		TIM_SetCompare4(TIM2, ccr);
	}
	else
	{
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
		GPIO_ResetBits(GPIOD, GPIO_Pin_14);
		TIM_SetCompare3(TIM2, -ccr);
		TIM_SetCompare4(TIM2, -ccr);
	}
}
