#ifndef __SERVO_H__

#define __SERVO_H__


void Servo_Init(void);
float Servo_GetAngle(void);
void Servo_Set(float Angle);

void Servo_Left(void);      //固定30左
void Servo_Right(void);     //固定30右

void Servo_TurnLeft(void);      //向左15
void Servo_TurnRight(void);     //向右15

#endif
