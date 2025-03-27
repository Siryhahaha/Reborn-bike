#ifndef __WHEEL_H__

#define __WHEEL_H__

extern uint8_t Wheel_EN;
extern uint8_t Wheel_DIR;
 
void Wheel_Init(void);
int Wheel_Limit(int PWM);
void Wheel_Load(int PWM);
void Wheel_Stop_Detect(void);

#endif
