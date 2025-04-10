#ifndef __MOTOR_H__
#define __MOTOR_H__

extern float motor_duty;

void MOTOR_Init(void);
void MOTOR_SetDuty(float duty);
void MOTOR_SetDir(int Dir);
void MOTOR_SpeedDelta(float d);

#endif
