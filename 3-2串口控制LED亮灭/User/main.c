#include "stm32f10x.h"                  // Device header
#include "LED.h"
#include "Delay.h"
#include "USART.h"

extern uint8_t LEDstate;
int main(void){
	
	LED1_Init();
	USART1_Init();
	
	while(1){
		if (LEDstate){
			LED1_Turn();
			Delay_s(1);
		}
	}
}
