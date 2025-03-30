#ifndef __MPU6050_H__
#define __MPU6050_H__

#define DEFAULT_MPU_HZ  (20)
#define Q30  1073741824.0f

int MPU6050_DMP_Init(void);
int MPU6050_DMP_Get_Data(float *pitch, float *roll, float *yaw);
int MPU6050_Get_Gyroscope(short *gyro);
int MPU6050_Get_Accelerometer(short *accel);

#endif
