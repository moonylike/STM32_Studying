#include "stm32f10x.h"                  // Device header
#include "Delay.h"

/*	默认低电平，按下高电平

	key1 PA0
	Key2 PF1
	Key3 PC13
	Key4 PE6
*/

//Key1初始化
void Key1_Init (void){
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE);	//开时钟
	GPIO_InitTypeDef GPIO_InitStructureA0;
	GPIO_InitStructureA0.GPIO_Mode =  GPIO_Mode_IPD;		//下拉输入模式
	GPIO_InitStructureA0.GPIO_Pin = GPIO_Pin_0;	
	GPIO_InitStructureA0.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructureA0);
}
//检测Key1被按下
uint8_t Key1_GetNum(void){
	uint8_t KeyNum = 0;
	if (GPIO_ReadInputDataBit (GPIOA, GPIO_Pin_0) == 1){	//当按键按下时
		Delay_ms(20);										//等待20ms，消除抖动
		while (GPIO_ReadInputDataBit (GPIOA, GPIO_Pin_0) == 1);//等待至按键松开
		Delay_ms(20);										//消除抖动
		KeyNum = 1;
	}
	return KeyNum;
}
//初始化任意一个按键
void Keyn_Init (uint8_t n){
	uint32_t x;
	uint16_t y;
	GPIO_TypeDef * z;
	if ( n == 1){						//Key1
		x = RCC_APB2Periph_GPIOA;
		y = GPIO_Pin_0;
		z = GPIOA;
	}else if ( n == 2){					//Key2
		x = RCC_APB2Periph_GPIOF;
		y = GPIO_Pin_1;
		z = GPIOF;
	}else if ( n == 3){					//Key3
		x = RCC_APB2Periph_GPIOC;
		y = GPIO_Pin_13;
		z = GPIOC;
	}else if ( n == 4){					//Key4
		x = RCC_APB2Periph_GPIOE;
		y = GPIO_Pin_6;
		z = GPIOE;
	}else {
		return;
	}
	RCC_APB2PeriphClockCmd(x, ENABLE);				//开时钟
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPD;	//下拉输入模式
	GPIO_InitStructure.GPIO_Pin = y;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(z, &GPIO_InitStructure);
}
//检测任意一个按键被按下
uint8_t Keyn_GetNum(uint8_t n){
	uint16_t y;
	GPIO_TypeDef * z;
	if ( n == 1){						//Key1
		y = GPIO_Pin_0;
		z = GPIOA;
	}else if ( n == 2){					//Key2
		y = GPIO_Pin_1;
		z = GPIOF;
	}else if ( n == 3){					//Key3
		y = GPIO_Pin_13;
		z = GPIOC;
	}else if ( n == 4){					//Key4
		y = GPIO_Pin_6;
		z = GPIOE;
	}else {
		return 0;						//若输入不合法，则返回0
	}
	uint8_t KeyNum = 0;
	if (GPIO_ReadInputDataBit (z, y) == 1){			//当按键按下时
		Delay_ms(20);								//等待20ms，消除抖动
		while (GPIO_ReadInputDataBit (z, y) == 1);	//等待至按键松开
		Delay_ms(20);								//消除抖动
		KeyNum = 1;
	}
	return KeyNum;
}
