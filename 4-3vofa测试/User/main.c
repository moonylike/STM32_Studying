#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "USART.h"
#include "IR_ADC.h"
#include "TIM.h"
#include "IR_DMA.h"

int main(void){
	
	USART1_Init();
	IRADCInit();
	IRDMAInit();
//	TIM2_Init();
	ADC_SoftwareStartConvCmd (ADC1, ENABLE);			//软件触发ADC

	while(1){
		printf ("%d,%d,%d,%d\n", Value[0], Value[1], Value[2], Value[3]);
		Delay_ms (50);
	}
}
