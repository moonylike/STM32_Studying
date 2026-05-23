#include "stm32f10x.h"                  // Device header
#include "USART.h"
#include "IR_ADC.h"
#include "TIM.h"
#include "IR_DMA.h"

int main(void){
	
	USART1_Init();
	IRADCInit();
	IRDMAInit();
	TIM2_Init();
	
	while(1){
				
	}
}
