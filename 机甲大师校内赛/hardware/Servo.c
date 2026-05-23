#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Servo_Init(void)
{
	PWM_Init();
} 

void Servo_SetAngle1(float Angle1)
{
	PWM_SetCompare5(Angle1/180*1000);
}
void Servo_SetAngle2(float Angle2)
{
	PWM_SetCompare6(Angle2/180*1000);
}
