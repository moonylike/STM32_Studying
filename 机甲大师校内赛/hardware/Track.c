#include "stm32f10x.h"                  // Device header
#include "ADC.h"
#include "PID.h"
#include "Motor.h"
#include "OLED.h"

PID_TypeDef pid;
int32_t dir = 0;
int32_t division=1200;
uint16_t Lit[9];
uint16_t Last_Lit[9];

uint16_t min(uint16_t x, uint16_t y)
{
	if(x>y) return y;
	else return x;
}

uint16_t max(uint16_t x, uint16_t y)
{
	if(x>y) return x;
	else return y;
}

void Track_Init(void)
{
	PID_Init(&pid, 120, 0, -20, 0, 10000);
	Lit[1] = 0;
	Lit[2] = 0;
	Lit[3] = 0;
	Lit[4] = 0;
	Lit[5] = 0;
	Lit[6] = 0;
	Lit[7] = 0;
	Lit[8] = 0;
	Last_Lit[1] = 0;
	Last_Lit[2] = 0;
	Last_Lit[3] = 0;
	Last_Lit[4] = 0;
	Last_Lit[5] = 0;
	Last_Lit[6] = 0;
	Last_Lit[7] = 0;
	Last_Lit[8] = 0;
}

void check(void)
{
	OLED_ShowSignedNum(4,1,(int32_t)(pid.Maxout),5);
}

void PID_Track(void)
{
	static float Error = 0.0;
	
	int16_t j=1;
	
	for(j=1; j<=8; j++)
		Last_Lit[j] = Lit[j];
	
	Lit[1] = forward1_GetValue();
	Lit[2] = forward2_GetValue();
	Lit[3] = forward3_GetValue();
	Lit[4] = forward4_GetValue();
	Lit[5] = forward5_GetValue();
	Lit[6] = forward6_GetValue();
	Lit[7] = forward7_GetValue();
	Lit[8] = forward8_GetValue();
	
	if(Lit[8]) Error = -50.0;
	if(Lit[7]) Error = -40.0;
	if(Lit[6]) Error = -35.0;
	if(Lit[3]) Error = 35.0;
	if(Lit[2]) Error = 40.0;
	if(Lit[1]) Error = 50.0;
	if(Lit[5]) Error = 0.0;
	if(Lit[4]) Error = 0.0;
		
	Pos_PID_Calc(&pid, 0, Error);
	if(Error < 20.0 && Error> -20.00)
	{
		SetLeftSpeed(7000.0+1*pid.out);
		SetRightSpeed(7000.0-1*pid.out);
	}
	else
	{
		SetLeftSpeed(4000+1*pid.out);
		SetRightSpeed(4000-1*pid.out);
	}
}

