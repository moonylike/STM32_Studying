#ifndef __PID_H
#define __PID_H

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

void PID_Init(PID_TypeDef *pid, float p, float i, float d, float maxiout, float mxout);
void Pos_PID_Calc(PID_TypeDef *pid, float ref, float fdb);
void Add_PID_Calc(PID_TypeDef *pid, float ref, float fdb);


#endif
