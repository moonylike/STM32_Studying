#include "stm32f10x.h"                  // Device header
#include "OLED.h"

void USART1_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);	//开启GPIO和USART的时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;				//初始化PA9和PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//PA9为复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//PA10为上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;						//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//不开启硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//同时开启输出和输入
	USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//一位停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长为8比特
	USART_Init(USART1, &USART_InitStructure);
	
	USART_Cmd(USART1, ENABLE);							//使能USART1
}
//发送字符
void Serial_SendByte(uint16_t Byte){
	USART_SendData(USART1, Byte);//发送字符
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//等待至字符已发送
}
