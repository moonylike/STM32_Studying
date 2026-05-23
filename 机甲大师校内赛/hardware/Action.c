#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "Delay.h"
#include "ADC.h"



void Go_Ahead(){
	SetLeftSpeed(10000);
	SetRightSpeed(10000);
}
void Go_Back(){
	SetLeftSpeed(-10000);
	SetRightSpeed(-10000);
}
void Turn_Left(){
	SetLeftSpeed(-7500);
	SetRightSpeed(7500);
}
void Turn_Right(){
	SetRightSpeed(-7500);
	SetLeftSpeed(+7500);
}
void Car_Stop(){
	SetLeftSpeed(0);
	SetRightSpeed(0);
}
