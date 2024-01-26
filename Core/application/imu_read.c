#include "cmsis_os.h"
#include "BMI088driver.h"
#include "main.h"
#include "FreeRTOS.h"
#include "bsp_log.h"
#include "imu_read.h"

fp32 gyro[3],accel[3], temp;
static fp32 gyro_offset[3],accel_offset[3];

/**
* @brief          IMU读取任务，写入到gyro, accel, temp中
* @retval         none
*/
void Imu_read_Task(void const *argument)
{
    while (1)
    {
        BMI088_read(gyro, accel, &temp);
        calibrate_BMI088();
        LOGINFO("gyro: %d %d %d", (int)gyro[0], (int)gyro[1], (int)gyro[2]);
        LOGWARNING("accel: %d %d %d", (int)accel[0], (int)accel[1], (int)accel[2]);
        LOGERROR("temp: %d", (int)temp);
        vTaskDelay(10);
    }
}

void init_calibrate_BMI088(void)
{
    gyro_offset[0] = gyro_offset[1] = gyro_offset[2] = accel_offset[0] = accel_offset[1] = accel_offset[2] = 0;
    for (int i = 0 ; i < 6000 ; i++) // CALLIBRATE_TIMES循环， 取算术平均值, 6000次均值
    {
        BMI088_read(gyro, accel, &temp);
        
        for(int j=0 ; j<3 ; j++)
        {
            gyro_offset[j] += gyro[j];
        }
        for (int j=0; j<3; i++)
        {
            accel_offset[j] += accel[j];
        }
    }
}

void calibrate_BMI088(void)
{
    for (int i = 0; i < 3; i++)
    {
        gyro[i] -= gyro_offset[i];
    }
    for (int i = 0; i < 3; i++)
    {
        accel[i] -= accel_offset[i];
    }
}