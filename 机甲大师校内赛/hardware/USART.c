#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "Motor.h"

/**
  * @brief  初始化 USART1 作为直接连接蓝牙的外设
  * @param  无
  * @retval 无
  */
void LanYa_Init(void){
//以下为 USART1 初始化
	
//开启GPIO和USART的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	
//初始化PA9和PA10
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;					//PA9为复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;					//PA10为上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//初始化USART1
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;						//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//不开启硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//同时开启输出和输入
	USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//一位停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长为8比特
	USART_Init(USART1, &USART_InitStructure);
	
	USART_Cmd(USART1, ENABLE);										//使能USART1
	
//接收数据时产生中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);					//输入寄存器非空时产生中断
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;				//选择中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
//以下为机械臂初始化
	
//时基单元初始化
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM5, ENABLE);	//开启TIM5的时钟
	
	TIM_InternalClockConfig (TIM5);			//选择内部时钟
	
//时基单元初始化 50赫兹
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitInitStructure;
	TIM_TimeBaseInitInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分频：DIV1滤波效果最好，信号延时最大
	TIM_TimeBaseInitInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数模式
	TIM_TimeBaseInitInitStructure.TIM_Period = 10000 - 1; 				//自动重装（ARR）
	TIM_TimeBaseInitInitStructure.TIM_Prescaler = 144 - 1;				//预分频  （PSC）
	TIM_TimeBaseInitInitStructure.TIM_RepetitionCounter = 0;			//高级定时器才用，直接给0就好
	TIM_TimeBaseInit (TIM5, &TIM_TimeBaseInitInitStructure);
	
//CCR输出比较寄存器初始化
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);							//统一赋初始值
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//PWM输出
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//输出极性（翻转电平）
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//输出使能
	TIM_OCInitStructure.TIM_Pulse = 740;							//脉冲：CCR寄存器的值
	TIM_OC2Init (TIM5,&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_Pulse = 750;							//脉冲：CCR寄存器的值
	TIM_OC3Init (TIM5,&TIM_OCInitStructure);
	
//初始化 PA1 与 PA2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开时钟
	GPIO_InitTypeDef GPIO_InitStructureA12;
	GPIO_InitStructureA12.GPIO_Mode =  GPIO_Mode_AF_PP;		//复用推挽输出模式
	GPIO_InitStructureA12.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructureA12.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructureA12);
	GPIO_InitStructureA12.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructureA12);
	
	TIM_Cmd (TIM5, ENABLE);								//开启定时器
}

/**
  * @brief  向蓝牙发送单一字节
  * @param  Byte:要发送的字节
  * @retval 无
  */
void Serial_SendByte(uint16_t Byte){
	USART_SendData(USART1, Byte);									//发送字符
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//等待至字符已发送
}

/**
  * @brief  重定向 printf 函数到 USART1
  *		    通过 printf 实现 USART1 打印
  *			不可直接使用，通过使用 printf 函数实现
  */
int fputc (int ch, FILE *f){
	Serial_SendByte (ch);
	return ch;
}

/**
  * @brief  通过 USART1 向蓝牙发送字符串
  *			用法与 printf 完全一致
  *			但是更推荐使用这个函数而非 printf ,因为使用 printf 可能会造成莫名其妙的错误
  * @param  懂的都懂，无需多言
  * @retval 无
  */
void Serial_Printf (char *format, ...){
	char String[100];
	va_list arg;
	va_start (arg, format);
	vsprintf (String, format, arg);
	va_end (arg);
	for (uint8_t i = 0; String[i] !='\0'; i++){
		Serial_SendByte (String[i]);
	}
}

/**
  * @brief  通过 USART1 向蓝牙发送指令
  *			这个指令可以在小程序的显示屏上显示字符串
  * @param  Line: 行数，这个参数理论上最大为65535，但实际上最好不要超过显示屏长度的80%
  *	@param  Column：列数，这个参数理论上最大为65535，但实际上最好不要超过显示屏长度的80%
  *	@param  Size：字号，表示显示的字符串的大小，其值为一个像素的大小
  *			推荐设置为18
  *	@param  String:要显示的字符串，比如 "Hello World!"
  *			注意要使用双引号括起来
  * @retval 无
  */
void Serial_DisplayPrintf (uint16_t Line, uint16_t Column, uint8_t Size, char *String){
	Serial_Printf ("[display,%d,%d,%s,%d]",Column, Line, String, Size);
}

/**
  * @brief  清屏，清除小程序的显示屏
  * @param  无
  * @retval 无
  */
void Serial_DisplayClear (void){
	Serial_Printf ("[display-clear]");
}

void Key1IT_Init (void){
//初始化 按键1
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE);	//开时钟
	GPIO_InitTypeDef GPIO_InitStructureA0;
	GPIO_InitStructureA0.GPIO_Mode =  GPIO_Mode_IPD;		//下拉输入模式
	GPIO_InitStructureA0.GPIO_Pin = GPIO_Pin_0;	
	GPIO_InitStructureA0.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructureA0);
	
//开启按键中断
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_AFIO, ENABLE);		//开时钟
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);	//EXTI输入
	
	EXTI_InitTypeDef EXTI_InitStructure;						//EXTI输出
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//下降沿触发
	EXTI_Init (&EXTI_InitStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//响应优先级
	NVIC_Init (&NVIC_InitStructure);
}


//下面的是有关接收文本数据包（蓝牙指令）的代码

/**
  * @brief	接收处理状态标志位
  *			表示单片机是否处于处理蓝牙指令的状态
  * @retval	0：不处于
  *			1：处于
  */
uint8_t Serial_RxFlag = 0;

void EXTI0_IRQHandler (void){
	if (EXTI_GetITStatus(EXTI_Line0) == SET){
		EXTI_ClearITPendingBit(EXTI_Line0);		//清除寄存器
		SetLeftSpeed(0);
		SetRightSpeed(0);
		LanYa_Init();
		while(1){}	
	}
}

/**
  *	@brief	获取并暂存来自蓝牙的指令（数据）
  * @retval	懂的都懂，无需多言
  */
char Serial_RxPacket[100] = {0};

//USART1 中断函数
void USART1_IRQHandler(void){
	static uint8_t Serial_RxStatus = 0;									//数据接收状态机
	static uint8_t Serial_pRxStatus = 0;								//接收数据的个数
	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET){			//检验是否收到数据
		
		uint8_t RxData = USART_ReceiveData (USART1);					//读取数据，并清空标志位
		
		if (Serial_RxStatus == 0 && RxData == '['){
				Serial_RxStatus = 1;
				Serial_pRxStatus = 0;
		}else if (Serial_RxStatus == 1){
			if (RxData != ']') 
				Serial_RxPacket[Serial_pRxStatus++] = RxData;
			else{
				Serial_RxStatus = 0;
				Serial_RxPacket[Serial_pRxStatus] = '\0';
				Serial_RxFlag = 1;
				
				char *Tag = strtok (Serial_RxPacket, ",");
				if (strcmp (Tag, "key") == 0){							//接收到按键数据包
					char *Name = strtok (NULL, ",");
					char *Action = strtok (NULL, ",");
					if (strcmp (Name, "forward") == 0){
						if (strcmp (Action, "down") == 0){
							Serial_Printf ("[display,100,80,forward,25]");	//小车前进
							SetLeftSpeed(-9000);
							SetRightSpeed(-8000);
						}else if (strcmp (Action, "up") == 0){
							Serial_Printf ("[display-clear]");				//小车停止前进
							SetLeftSpeed(0);
							SetRightSpeed(0);
						}
					}else if (strcmp (Name, "back") == 0){
						if (strcmp (Action, "down") == 0){
							Serial_Printf ("[display,120,80,back,25]");		//小车后退	
							SetLeftSpeed(8000);
							SetRightSpeed(8000);
						}else if (strcmp (Action, "up") == 0){
							Serial_Printf ("[display-clear]");				//小车停止后退
							SetLeftSpeed(0);
							SetRightSpeed(0);
						}
					}else if (strcmp (Name, "left") == 0){
						if (strcmp (Action, "down") == 0){
							Serial_Printf ("[display,120,80,left,25]");		//小车左转
							SetLeftSpeed(-8000);
							SetRightSpeed(+8000);							
						}else if (strcmp (Action, "up") == 0){
							Serial_Printf ("[display-clear]");				//小车停止左转
							SetLeftSpeed(0);
							SetRightSpeed(0);
						}
					}else if (strcmp (Name, "right") == 0){
						if (strcmp (Action, "down") == 0){
							Serial_Printf ("[display,110,80,right,25]");	//小车右转	
							SetRightSpeed(-8000);
							SetLeftSpeed(+8000);
						}else if (strcmp (Action, "up") == 0){
							Serial_Printf ("[display-clear]");				//小车停止右转
							SetLeftSpeed(0);
							SetRightSpeed(0);
						}
                    }else if (strcmp (Name, "middle") == 0)		//机械臂竖直
						TIM_SetCompare3(TIM5, 750);
					else if (strcmp (Name, "straight") == 0)	//机械臂水平前伸
						TIM_SetCompare3(TIM5, 1000);
					else if (strcmp (Name, "down") == 0)		//机械臂向前下方
						TIM_SetCompare3(TIM5, 1200);
					else if (strcmp (Name, "release") == 0)		//机械爪松开
						TIM_SetCompare2(TIM5, 600);
					else if (strcmp (Name, "per-seize") == 0)	//机械爪预抓取
						TIM_SetCompare2(TIM5, 850);
					else if (strcmp (Name, "seize") == 0)		//机械爪抓取
						TIM_SetCompare2(TIM5, 887);
					else if (strcmp (Name, "MLeft") == 0)		//机械爪抓取
					{
						if (strcmp (Action, "down") == 0){
							Serial_Printf ("[display,110,80,right,25]");	//小车右转	
							SetRightSpeed(+5000);
							SetLeftSpeed(-5000);
						}else if (strcmp (Action, "up") == 0){
							Serial_Printf ("[display-clear]");				//小车停止右转
							SetLeftSpeed(0);
							SetRightSpeed(0);
						}
					}
					else if (strcmp (Name, "MRight") == 0)		
					{
						if (strcmp (Action, "down") == 0){
							Serial_Printf ("[display,110,80,right,25]");		
							SetRightSpeed(-5000);
							SetLeftSpeed(+5000);
						}else if (strcmp (Action, "up") == 0){
							Serial_Printf ("[display-clear]");				
							SetLeftSpeed(0);
							SetRightSpeed(0);
						}
					}
					else if (strcmp (Name, "MForward") == 0)		
					{
						if (strcmp (Action, "down") == 0){
							Serial_Printf ("[display,110,80,right,25]");	
							SetRightSpeed(-5000);
							SetLeftSpeed(-5000);
						}else if (strcmp (Action, "up") == 0){
							Serial_Printf ("[display-clear]");				
							SetLeftSpeed(0);
							SetRightSpeed(0);
						}
					}
					else if (strcmp (Name, "MBack") == 0)		
					{
						if (strcmp (Action, "down") == 0){
							Serial_Printf ("[display,110,80,right,25]");	
							SetRightSpeed(+5000);
							SetLeftSpeed(+5000);
						}else if (strcmp (Action, "up") == 0){
							Serial_Printf ("[display-clear]");				
							SetLeftSpeed(0);
							SetRightSpeed(0);
						}
					}
				}else if (strcmp (Tag, "slider") == 0){					//接收到滑杆数据包
					char *Name = strtok (NULL, ",");
					char *Value = strtok (NULL, ",");
					uint16_t IntValue = atoi(Value);			//Value转整型
					if (strcmp (Name, "wide") == 0){
						TIM_SetCompare2(TIM5, IntValue);		//调整机械爪宽度
					}else if (strcmp (Name, "location") == 0){
						TIM_SetCompare3(TIM5, IntValue);		//调整机械臂位置
					}
					/*
					uint8_t IntValue = atoi(Value);//Value转整型
					float FloatValue = atof(Value);//Value转浮点型
					接下来使用方法同上，但目前暂时不添加有效值
					*/	
				}else if (strcmp (Tag, "joystick") == 0){				//接收到遥杆数据包
					int8_t LH = atoi(strtok (NULL, ","));				//左摇杆横向值
					int8_t LV = atoi(strtok (NULL, ","));				//左摇杆纵向值
					int8_t RH = atoi(strtok (NULL, ","));				//右摇杆横向值
					int8_t RV = atoi(strtok (NULL, ","));				//右摇杆纵向值
					/*
					接下来使用方法同上，但目前暂时不添加有效值
					*/
				}
				Serial_RxFlag = 0;
			}
		}
	}
}
