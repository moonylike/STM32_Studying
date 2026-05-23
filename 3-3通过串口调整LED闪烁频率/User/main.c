#include "stm32f10x.h"                  // Device header
#include "LED.h"
#include "TIM.h"
#include "USART.h"
#include "OLED.h"

int main(void){
	
	LED1_Init();
	TIM2_Init();
	USART1_Init();
	OLED_Init();
	OLED_ShowString(1, 1, "Frequency:");
	OLED_ShowString(2, 3, "01.00Hz");

	while(1){
		
	}
}
