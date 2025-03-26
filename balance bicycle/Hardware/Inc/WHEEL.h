#ifndef __WHEEL_H__

#define __WHEEL_H__

extern uint8_t Wheel_EN;
extern uint8_t Wheel_DIR;
 

void Limit(int *PWM);
void Load(int PWM);
void Stop_Detect(void);

#endif
