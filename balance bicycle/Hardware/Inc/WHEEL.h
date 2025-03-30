#ifndef __WHEEL_H__
#define __WHEEL_H__

void Wheel_Init(void);
void Wheel_SetDir(int Dir);
int Wheel_Limit(int PWM);
void Wheel_Load(int PWM);

#endif
