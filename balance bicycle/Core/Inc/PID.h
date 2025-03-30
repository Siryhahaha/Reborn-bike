#ifndef __PID_H__
#define __PID_H__

extern float med_angle,
             vertical_kp,
             vertical_kd,
             velocity_kp,
             velocity_ki;

int Vertical(float angle, float gyro);
int Velocity(int encoder_value);

#endif
