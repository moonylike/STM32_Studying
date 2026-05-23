#include "stm32f10x.h"                  // Device header

typedef struct
{
	float Kp;
	float Ki;
	float Kd;
	
	float Error;
	float Pre_Error;
	float Pre_Pre_Error;
	
	float integral;
	
	float Maxout;
	float MaxIout;//最大输出，最大积分输出
	
	float out;
	float Pout;
	float Iout;
	float Dout;
} PID_TypeDef;

void PID_Init(PID_TypeDef *pid, float p, float i, float d, float maxiout, float mxout)
{
	pid->Kp = p;
	pid->Ki = i;
	pid->Kd = d;
	pid->MaxIout = maxiout;
	pid->Maxout = mxout;
	pid->Error = 0;
	pid->Pre_Error = 0;
	pid->Pre_Pre_Error = 0;
	pid->integral = 0;
	pid->out = 0;
	pid->Pout = 0;
	pid->Iout = 0;
	pid->Dout = 0;
}

void Pos_PID_Calc(PID_TypeDef *pid, float ref, float fdb)
{
	pid->Pre_Error = pid->Error;
	pid->Error = ref-fdb;
	
	pid->Pout = pid->Kp * pid->Error;
	pid->Dout = pid->Kd * (pid->Error - pid->Pre_Error);
	
	pid->out = pid->Pout + pid->Iout + pid->Dout;
	
	if(pid->out > pid->Maxout) pid->out = pid->Maxout;
	else if(pid->out < -pid->Maxout) pid->out = -pid->Maxout;
}

void Add_PID_Calc(PID_TypeDef *pid, float ref, float fdb)
{
	pid->Pre_Pre_Error = pid->Pre_Error;
	pid->Pre_Error = pid->Error;
	pid->Error = ref-fdb;
	pid->out += pid->Kp*(pid->Error-pid->Pre_Error) + pid->Ki*pid->Error + pid->Kd*(pid->Error+pid->Pre_Pre_Error-2*pid->Pre_Error);
	
	if(pid->out > pid->Maxout) pid->out = pid->Maxout;
	else if(pid->out < -pid->Maxout) pid->out = -pid->Maxout;
}
