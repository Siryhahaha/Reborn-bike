#ifndef __WHEEL_H__
#define __WHEEL_H__

extern uint8_t wheel_en;

void Wheel_Init(void);
void Wheel_SetDir(int Dir);
int Wheel_Limit(int PWM);
void Wheel_Load(int PWM);

#endif
