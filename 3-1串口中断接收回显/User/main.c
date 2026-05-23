#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "USART.h"
#include "LED.h"

int main(void){
	LED1_Init ();
	LED1_Turn ();
//	OLED_Init();
//	USART1_Init();
	
	while(1){
		
	}
}
