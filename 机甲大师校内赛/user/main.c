#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "Motor.h"
#include "Action.h"
#include "ADC.h"
#include "Track.h"
#include "USART.h"
#include "TIM.h"
#include <stdbool.h>

uint32_t high=600,low=300;
uint16_t Possible_T=0;
uint32_t cnt=500;
uint16_t Possible_Die=0;
bool Die[30]={0};
uint16_t DieFlag=0;

// 传感器状态结构体
typedef struct {
    bool forwardLeft1;
    bool forwardLeft2;
	bool forwardLeft3;
    bool forwardLeft4;
    bool forwardRight1;
    bool forwardRight2;
	bool forwardRight3;
    bool forwardRight4;
    bool backLeft;
    bool backRight;
	bool middleLeft;
	bool middleRight;
} SensorState;

void Left_hand(void);

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Key1IT_Init();
	Motor_Init();
	ADCDetect_Init();
	Track_Init();
	OLED_Init();
	myTIM_Init();
	Left_hand();
	
	while (1)
	{
		PID_Track();
		Left_hand();
	}
}

// 获取当前传感器状态
SensorState GetSensorState(void) {
    SensorState state;
    state.forwardLeft1 = (forward4_GetValue() == 1);
    state.forwardLeft2 = (forward3_GetValue() == 1);
	state.forwardLeft3 = (forward2_GetValue() == 1);
    state.forwardLeft4 = (forward1_GetValue() == 1);
    state.forwardRight1 = (forward5_GetValue() == 1);
    state.forwardRight2 = (forward6_GetValue() == 1);
	state.forwardRight3 = (forward7_GetValue() == 1);
    state.forwardRight4 = (forward8_GetValue() == 1);
    state.backLeft = (backLeft_GetValue() > high);                                                               
    state.backRight = (backRight_GetValue() > high);
	state.middleLeft = (middleLeft_GetValue() > high);                                                               
    state.middleRight = (middleRight_GetValue() > high);
    return state;
}

// 转弯直到检测到黑线
void TurnUntilLineDetected(void (*turn_function)(void)) {
    turn_function();
	if(DieFlag)
	{
		while(1)
		{
			if(forward4_GetValue() || forward6_GetValue()) return;
		}
	}
	Delay_ms(300);
    while(1) {
        if(forward4_GetValue()) {
            break;
        }
    }
	dir=0;
}

//检查掉头
bool IsTurnAround(const SensorState*s){
	   return DieFlag>0;
   }
// 检查T字或十字路口
bool IsTJunction(const SensorState* s) {
     return s->forwardLeft3 && s->forwardRight3;
}

// 检查停止位
bool IsStopPosition(const SensorState* s) {
    return (s->forwardRight2 && s->forwardLeft1);
}

// 检查左转
bool IsLeftTurn(const SensorState* s) {
    return s->forwardLeft4 && !s->forwardRight4 && s->forwardLeft3 && !s->forwardRight3;
}

// 检查左T路口
bool IsTLeftTurn(const SensorState* s) {
    return s->forwardLeft3 && !s->forwardRight3;
}


// 检查右转
bool IsRightTurn(const SensorState* s) {
    return !s->forwardLeft4 && s->forwardRight4;
}

// 检查右T路口
bool IsTRightTurn(const SensorState* s) {
    return !s->forwardLeft4 && s->forwardRight4 && !s->forwardLeft3 && s->forwardRight3 && 
		!s->forwardLeft2 && s->forwardRight2 && !s->forwardLeft1;
}

// 左手定则主函数
void Left_hand(void) {
    while(1) {
        
        SensorState s = GetSensorState();
		OLED_ShowString(1,1,"/|\\");
		OLED_ShowString(2,2,"|");
		OLED_ShowString(3,2,"|");
		OLED_ShowString(4,2,"|");
        // 更新传感器状态
		//掉头
		if(IsTurnAround(&s)) {
			OLED_ShowString(1,1,"   ");
			OLED_ShowString(2,2," ");
			OLED_ShowString(3,2," ");
			OLED_ShowString(4,2," ");
			OLED_ShowString(1,1,"--->");
			TurnUntilLineDetected(Turn_Right);
			OLED_ShowString(1,1,"    ");
		}
		
        // T字+十字路口
       if(IsTJunction(&s)) { 
            Possible_T =1;
			cnt=0;
			while(1)
			{
				SetLeftSpeed(4500);
				SetRightSpeed(4500);
				s = GetSensorState();
				if(cnt>40&&cnt<500)
				{
					OLED_ShowString(1,1,"   ");
					OLED_ShowString(2,2," ");
					OLED_ShowString(3,2," ");
					OLED_ShowString(4,2," ");
					OLED_ShowString(1,1,"----");
					Car_Stop();
					while(1);
				}
				if(s.backLeft||s.backRight) cnt=500;
				//T字
				if(s.backLeft && !s.forwardLeft1) {
					OLED_ShowString(1,1,"   ");
					OLED_ShowString(2,2," ");
					OLED_ShowString(3,2," ");
					OLED_ShowString(4,2," ");
					OLED_ShowString(1,1,"<---");
					TurnUntilLineDetected(Turn_Left);
					OLED_ShowString(1,1,"    ");
					Possible_T=0;
					break;
				}
				//十字
				else if(s.backLeft && s.forwardLeft1) {
					OLED_ShowString(1,1,"   ");
			        OLED_ShowString(2,2," ");
					OLED_ShowString(3,2," ");
					OLED_ShowString(4,2," ");
					OLED_ShowString(1,1,"<---");
					TurnUntilLineDetected(Turn_Left);
					OLED_ShowString(1,1,"    ");
					Possible_T=0;
					break;
				}
			}
			OLED_ShowString(1,1,"/|\\");
			OLED_ShowString(2,2,"|");
			OLED_ShowString(3,2,"|");
			OLED_ShowString(4,2,"|");
		}
        // 左转或左T路口处理
        else if(IsLeftTurn(&s)) {
			cnt=0;
			OLED_ShowString(1,1,"   ");
			OLED_ShowString(2,2," ");
			OLED_ShowString(3,2," ");
			OLED_ShowString(4,2," ");
			OLED_ShowString(1,1,"--->");
			while(1)
			{
				SetLeftSpeed(4500);
				SetRightSpeed(4500);
				s = GetSensorState();
				if(cnt>40&&cnt<500)
				{
					OLED_ShowString(1,1,"   ");
			        OLED_ShowString(2,2," ");
					OLED_ShowString(3,2," ");
					OLED_ShowString(4,2," ");
					OLED_ShowString(1,1,"----");
					Car_Stop();
					while(1);
				}
				if(s.backLeft||s.backRight) cnt=500;
				if(s.backLeft) {
					TurnUntilLineDetected(Turn_Left);
					OLED_ShowString(1,1,"    ");
					break;
				}
            }
			OLED_ShowString(1,1,"/|\\");
			OLED_ShowString(2,2,"|");
			OLED_ShowString(3,2,"|");
			OLED_ShowString(4,2,"|");
		}
		PID_Track();
	}
}

void TIM6_IRQHandler(void)
{
	uint16_t i=1;
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
	{
		SensorState s = GetSensorState();
		Die[0] = !s.forwardLeft4 && !s.forwardRight4 && !s.forwardLeft3 && !s.forwardRight3 && 
		!s.forwardLeft2 && !s.forwardRight2 && !s.forwardLeft1;
		for(i=22; i>=1 ;i--)
			Die[i]=Die[i-1];
		DieFlag=1;
		for(i=0; i<23; i++)
			if(!Die[i])
			{
				DieFlag=0;
				break;
			}
		cnt++;
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}
}
