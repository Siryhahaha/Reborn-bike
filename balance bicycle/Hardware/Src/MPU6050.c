#include <math.h>
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "MPU6050.h"

#define MPL_LOGE(...)   do {} while (0)
#define MPL_LOGI(...)   do {} while (0)

struct platform_data_s {
    signed char orientation[9];
};

/* The sensors can be mounted onto the board in any orientation. The mounting
 * matrix seen below tells the MPL how to rotate the raw data from the
 * driver(s).
 * TODO: The following matrices refer to the configuration on internal test
 * boards at Invensense. If needed, please modify the matrices to match the
 * chip-to-body matrix for your particular set up.
 */
static struct platform_data_s gyro_pdata = {
    .orientation = { 1, 0, 0,
                     0, 1, 0,
                     0, 0, 1}
};

static unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;		// error
    return b;
}

/** Converts an orientation matrix made up of 0,+1,and -1 to a scalar representation.
* @param[in] mtx Orientation matrix to convert to a scalar.
* @return Description of orientation matrix. The lowest 2 bits (0 and 1) represent the column the one is on for the
* first row, with the bit number 2 being the sign. The next 2 bits (3 and 4) represent
* the column the one is on for the second row with bit number 5 being the sign.
* The next 2 bits (6 and 7) represent the column the one is on for the third row with
* bit number 8 being the sign. In binary the identity matrix would therefor be:
* 010_001_000 or 0x88 in hex.
*/
unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx)
{

    unsigned short scalar;

    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;

    return scalar;
}


// static inline int run_self_test(void)
// {
//     int result;
//     long gyro[3], accel[3];

//     result = mpu_run_self_test(gyro, accel);

//     if (result == 0x3) {
// 	    MPL_LOGI("Passed!\n");
//         MPL_LOGI("accel: %7.4f %7.4f %7.4f\n",
//                     accel[0]/65536.f,
//                     accel[1]/65536.f,
//                     accel[2]/65536.f);
//         MPL_LOGI("gyro: %7.4f %7.4f %7.4f\n",
//                     gyro[0]/65536.f,
//                     gyro[1]/65536.f,
//                     gyro[2]/65536.f);
//         /* Test passed. We can trust the gyro data here, so now we need to update calibrated data*/


//         /* Push the calibrated data to the MPL library.
//          *
//          * MPL expects biases in hardware units << 16, but self test returns
// 		 * biases in g's << 16.
// 		 */
//     	unsigned short accel_sens;
//     	float gyro_sens;

//         mpu_get_gyro_sens(&gyro_sens);
// 		gyro[0] = (long) (gyro[0] * gyro_sens);
// 		gyro[1] = (long) (gyro[1] * gyro_sens);
// 		gyro[2] = (long) (gyro[2] * gyro_sens);
// 		mpu_set_gyro_bias_reg(gyro);

// 		mpu_get_accel_sens(&accel_sens);
// 		accel[0] *= accel_sens;
// 		accel[1] *= accel_sens;
// 		accel[2] *= accel_sens;
// 		mpu_set_accel_bias_6050_reg(accel);
		
//         return 0;
//     }
//     else {
//             if (!(result & 0x1))
//                 MPL_LOGE("Gyro failed.\n");
//             if (!(result & 0x2))
//                 MPL_LOGE("Accel failed.\n");
//             if (!(result & 0x4))
//                 MPL_LOGE("Compass failed.\n");
//             return -1;
//      }
// }


int MPU6050_DMP_Init(void)
{
    int ret;
    struct int_param_s int_param;

    //初始化MPU6050
    ret = mpu_init(&int_param);
    if (ret != 0)
        return 1;

    //设置传感器
    ret = mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);
    if (ret != 0)
        return 2;
    
    //设置fifo
    ret = mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);
    if (ret != 0)
        return 3;

    //设置采样率
    ret = mpu_set_sample_rate(DEFAULT_MPU_HZ);
    if (ret != 0)
        return 4;
    
    //加载DMP固件
    ret = dmp_load_motion_driver_firmware();
    if (ret != 0)
        return 5;
    
    //设置陀螺仪方向
    ret = dmp_set_orientation(
        inv_orientation_matrix_to_scalar(gyro_pdata.orientation));
    if (ret != 0)
        return 6;

    //设置DMP功能
    ret  = dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
        DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
        DMP_FEATURE_GYRO_CAL);
    if (ret != 0)
        return 7;

    //设置输出速率
    ret = dmp_set_fifo_rate(DEFAULT_MPU_HZ);
    if (ret != 0)
        return 8;
    
    //自检
    // ret = run_self_test();
    // if (ret != 0)
    //     return 9;
    
    //使能DMP
    ret = mpu_set_dmp_state(1);
    if (ret != 0)
        return 10;

    return 0;
}

int MPU6050_DMP_Get_Date(float *pitch, float *roll, float *yaw)
{
    float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
    short gyro[3];
    short accel[3];
    long quat[4];
    unsigned long timestamp;
    short sensors;
    unsigned char more;
    if(dmp_read_fifo(gyro, accel, quat, &timestamp, &sensors, &more))
    {
        return -1;
    }

    if(sensors & INV_WXYZ_QUAT)
    {
        q0 = quat[0] / Q30;
        q1 = quat[1] / Q30;
        q2 = quat[2] / Q30;
        q3 = quat[3] / Q30;

        *pitch = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3; // pitch
        *roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3; // roll
        *yaw = atan2(2 * (q0 * q3 + q1 * q2), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.3; // yaw
    }

    return 0;
}
