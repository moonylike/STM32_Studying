#ifndef __MOTOR_H
#define __MOTOR_H

extern uint32_t high,low;

void Motor_Init(void);
void SetLeftSpeed(int32_t ccr);
void SetRightSpeed(int32_t ccr);

#endif
