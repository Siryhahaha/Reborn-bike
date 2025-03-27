#ifndef __MPU6050_H__
#define __MPU6050_H__

#define DEFAULT_MPU_HZ  (20)
#define Q30  1073741824.0f

int MPU6050_DMP_Init(void);
int MPU6050_DMP_Get_Date(float *pitch, float *roll, float *yaw);

#endif
