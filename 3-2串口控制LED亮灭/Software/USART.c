#include "stm32f10x.h"                  // Device header

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
	
	USART_Cmd(USART1, ENABLE);						//使能USART1
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//输入寄存器非空时产生中断
	
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2);//分组
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
}

void Serial_SendByte(uint16_t Byte){
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

uint8_t LEDstate = 0;

//收到LEDON开始闪烁，收到LEDOFF熄灯
void USART1_IRQHandler(void){
	static int8_t stateflag = 0;
	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET){
		if (stateflag == 0 && USART_ReceiveData (USART1) == 'L'){
			stateflag ++;
		}else if (stateflag == 1 && USART_ReceiveData (USART1) == 'E'){
			stateflag ++;
		}else if (stateflag == 2 && USART_ReceiveData (USART1) == 'D'){
			stateflag ++;
		}else if (stateflag == 3 && USART_ReceiveData (USART1) == 'O'){
			stateflag ++;
		}else if (stateflag == 4){
			if (USART_ReceiveData (USART1) == 'N'){
				LEDstate = 1;
				stateflag = 0;
			}else if (USART_ReceiveData (USART1) == 'F'){
			stateflag ++;
			}
		}else if (stateflag == 5 && USART_ReceiveData (USART1) == 'F'){
			LEDstate = 0;
			GPIO_SetBits (GPIOB, GPIO_Pin_0);
			stateflag = 0;
		}
	}
	
}
