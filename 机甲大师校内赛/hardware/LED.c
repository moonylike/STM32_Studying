#include "stm32f10x.h"                  // Device header

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;//定义模式配置结构体
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//通用推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;//PB0口
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//50MHZ
	GPIO_Init(GPIOB,&GPIO_InitStructure);//模式配置
	GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);//熄灭
}
void LED1_ON(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
}
void LED1_OFF(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
}
void LED2_ON(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
}
void LED2_OFF(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
}
