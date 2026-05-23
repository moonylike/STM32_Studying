#include "stm32f10x.h"                  // Device header
#include "USART.h"
#include "IR_ADC.h"
#include "TIM.h"

int main(void){
	
	IRADCInit();
	USART1_Init();
	TIM2_Init();

	while(1){
				
	}
}
