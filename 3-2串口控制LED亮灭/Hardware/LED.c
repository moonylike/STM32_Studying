#include "stm32f10x.h"                  // Device header

/*	低电平点亮，高电平熄灭

	LED1 PB0
	LED2 PB1
	LED3 PB5
	LED4 PB4***
	LED5 PF11
	LED6 PF12
*/

//初始化LED1
void LED1_Init (void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//开时钟
	GPIO_InitTypeDef GPIO_InitStructureB0;
	GPIO_InitStructureB0.GPIO_Mode =  GPIO_Mode_Out_PP;		//推挽输出模式
	GPIO_InitStructureB0.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructureB0.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructureB0);
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
}
//翻转LED1
void LED1_Turn (void){
	if (GPIO_ReadOutputDataBit (GPIOB, GPIO_Pin_0) == 0){	//如果为低电平
		GPIO_SetBits (GPIOB, GPIO_Pin_0);					//就设置为高电平
	}else{									//否则
		GPIO_ResetBits (GPIOB, GPIO_Pin_0);	//就设置为低电平
}
}

//对任意LED初始化
void LEDn_Init ( uint8_t n ){
	uint32_t x;
	uint16_t y;
	GPIO_TypeDef * z;
	if ( n == 1){						//LED1
		x = RCC_APB2Periph_GPIOB;
		y = GPIO_Pin_0;
		z = GPIOB;
	}else if ( n == 2){					//LED2
		x = RCC_APB2Periph_GPIOB;
		y = GPIO_Pin_1;
		z = GPIOB;
	}else if ( n == 3){					//LED3
		x = RCC_APB2Periph_GPIOB;
		y = GPIO_Pin_5;
		z = GPIOB;
	}else if ( n == 4){					//LED4	注意：使用LED4前必须重映射PB4
		x = RCC_APB2Periph_GPIOB;
		y = GPIO_Pin_4;
		z = GPIOB;
	}else if ( n == 5){					//LED5
		x = RCC_APB2Periph_GPIOF;
		y = GPIO_Pin_11;
		z = GPIOF;
	}else if ( n == 6){					//LED6
		x = RCC_APB2Periph_GPIOF;
		y = GPIO_Pin_12;
		z = GPIOF;
	}else {
		return;
	}
	RCC_APB2PeriphClockCmd(x, ENABLE);	//开时钟
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;		//推挽输出模式
	GPIO_InitStructure.GPIO_Pin = y;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(z, &GPIO_InitStructure);
	GPIO_SetBits(z, y);
}
//翻转任意一个LED
void LEDn_Turn ( uint8_t n){
	uint16_t y;
	GPIO_TypeDef * z;
	if ( n == 1){
		z = GPIOB;
		y = GPIO_Pin_0;
	}else if ( n == 2){
		z = GPIOB;
		y = GPIO_Pin_1;
	}else if ( n == 3){
		z = GPIOB;
		y = GPIO_Pin_5;
	}else if ( n == 4){
		z = GPIOB;
		y = GPIO_Pin_4;
	}else if ( n == 5){
		z = GPIOF;
		y = GPIO_Pin_11;
	}else if ( n == 6){
		z = GPIOF;
		y = GPIO_Pin_12;
	}else {
		return;
	}
	if (GPIO_ReadOutputDataBit (z, y) == 0){//如果为低电平
		GPIO_SetBits (z, y);				//就设置为高电平
	}else{									//否则
		GPIO_ResetBits (z, y);				//就设置为低电平
}
}

//预点亮LED4，因为LED4的引脚默认是调试脚，和LED功能冲突，所以要先使用重映射
void LED4_PerInit (void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //使能AFIO时钟
	GPIO_PinRemapConfig( GPIO_Remap_SWJ_JTAGDisable, ENABLE);//重映射B4
}
