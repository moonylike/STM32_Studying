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
	
	
//接收数据时产生中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);		//输入寄存器非空时产生中断
	
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2);	//分组
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	//选择中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	

}
//发送字符
void Serial_SendByte(uint16_t Byte){
	USART_SendData(USART1, Byte);//发送字符
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//等待至字符已发送
}

//USART1 中断函数

uint8_t ReceiveData = 0;
void FrequencyCount(void){
	uint8_t f1 = 10 / ReceiveData;						//整数频率
	double frequency = 10.0 / ReceiveData;				//总频率
	uint8_t f2 = (uint16_t)(frequency * 100) % 100;		//两位小数频率
	OLED_ShowNum (2, 3, f1, 2);
	OLED_ShowNum (2, 6, f2, 2);
}
uint8_t LEDstate = 1;

void USART1_IRQHandler(void){
	static int8_t stateflag = 0;
	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET){
		if (stateflag == 0){
			if (USART_ReceiveData (USART1) == 'L'){
				stateflag ++;
			}else if (USART_ReceiveData (USART1) == 'S'){
				stateflag = 7;
			}
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
		}else if (stateflag == 7 && USART_ReceiveData (USART1) == 'E'){
			stateflag ++;
		}else if (stateflag == 8 && USART_ReceiveData (USART1) == 'T'){
			stateflag ++;
		}else if (stateflag == 9){
			ReceiveData += USART_ReceiveData (USART1) - '0';
			ReceiveData *= 10;
			stateflag ++;
		}else if (stateflag == 10){
			ReceiveData += USART_ReceiveData (USART1) - '0';
			FrequencyCount();
			ReceiveData /= 2;
			TIM_SetAutoreload (TIM2, ReceiveData * 1000 -1);
			ReceiveData = 0;
			stateflag = 0;
		}
	}
}

/*	{
	buffer[currentPos] = USART_ReceiveData(USART1);
	currentPos++;
	// match the current buffer with the commands
	for (uint8_t i = 0; i < COMMAND_COUNT; i++) {
		Command command = commands[i];
		if (match_command(buffer, command.name)) {
			command.fn(buffer);
			clear_buffer();
			break;
		}
	}
	typedef struct {
	char name[10];
	void (*fn)(char[10]);
} Command;

const uint8_t COMMAND_COUNT = 3;
Command commands[COMMAND_COUNT];
void handle_led_on(char command[10]);
void handle_led_off(char command[10]);
void handle_set(char command[10]);

	commands[0] = (Command){
		.name = "LEDON",
		.fn = &handle_led_on
	};
	commands[1] = (Command){
		.name = "LEDOFF",
		.fn = &handle_led_off
	};
	commands[2] = (Command){
		.name = "SET%%", // 我们使用%来表示这个位置应该出现一个数字
		.fn = &handle_set
	};
	


void handle_led_on(char command[10]) {
	LEDstate = 1;
}

void handle_led_off(char command[10]) {
	LEDstate = 0;
}

void handle_set(char command[10]) {
	// command内容是明确的，就是SET%%
	// command[3] command[4]为数字
	uint8_t data = (
	    (command[3] - '0') * 10
	        + (command[4] - '0')
	    / 2);
	
	TIM_SetAutoreload (TIM2, data * 1000 -1);
	FrequencyCount();
}


char buffer[10];
uint8_t currentPos = 0;


int8_t match_command(char current_command[10], char pattern[10]) {
	uint8_t cmd_pos = 0;
	for (uint8_t i = 0; i < 10; i++) {
		char character = pattern[i];
		if (character == '\0') {
			return 1;
		}
		else if (character == '%') {
			char cmd_char = current_command[cmd_pos];
			cmd_pos++;
			if (cmd_char < '0' || cmd_char > '9') {
				return 0;
			}
		} else {
			if (character != current_command[cmd_pos]) {
				return 0;
			}
			cmd_pos++;
		}
	}
	return 1;
}


void clear_buffer(void) {
	for (uint8_t i = 0; i < 10; i++) {
		buffer[i] = 0;
	}
}

*/


