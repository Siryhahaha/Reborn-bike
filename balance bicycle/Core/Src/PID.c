#include "PID.h"

float med_angle     = 0,
      vertical_kp   = 600,
      vertical_kd   = 0,
      velocity_kp   = 0,
      velocity_ki   = 0;

int Vertical(float angle, float gyro)
{
    return vertical_kp * (angle - med_angle) + vertical_kd * gyro;
}

int Velocity(int speed)
{
    float a = 0.7;
    static int last_low_out = 0,
               integral = 0;

    int low_out = (1 - a) * speed + a * last_low_out;

    integral += low_out;
    if (integral > 10000)           integral = 10000;
    else if (integral < -10000)     integral = -10000;

    last_low_out = low_out;

    return velocity_kp * low_out + velocity_ki * integral;
}
