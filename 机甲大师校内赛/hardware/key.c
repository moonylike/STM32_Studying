#include "stm32f10x.h"                  // Device header
#include "Delay.h"
void Key2_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;//定义模式配置结构体
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//上拉输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;//PB0口
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//50MHZ
	GPIO_Init(GPIOC,&GPIO_InitStructure);//模式配置
	GPIO_SetBits(GPIOC,GPIO_Pin_13);//熄灭
}
void Key4_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;//定义模式配置结构体
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//上拉输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;//PB0口
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//50MHZ
	GPIO_Init(GPIOE,&GPIO_InitStructure);//模式配置
	GPIO_SetBits(GPIOE,GPIO_Pin_6);//熄灭
}
uint8_t Key_GetNum(void)
{
	uint8_t KeyNum=0;
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==Bit_SET)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==Bit_SET);//按下为高电平
		Delay_ms(20);
		KeyNum=1;
	}
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)==Bit_SET)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)==Bit_SET);
		Delay_ms(20);
		KeyNum=2;
	}
	return KeyNum;
}

