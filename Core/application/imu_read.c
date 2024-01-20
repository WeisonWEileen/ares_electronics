#include "cmsis_os.h"
#include "BMI088driver.h"
#include "main.h"
#include "FreeRTOS.h"
#include "bsp_log.h"

fp32 gyro[3],accel[3], temp;

/**
* @brief          IMU读取任务，写入到gyro, accel, temp中
* @retval         none
*/
void Imu_read_Task(void const *argument)
{
    while (1)
    {
        BMI088_read(gyro, accel, &temp);
        LOGINFO("gyro: %d %d %d", (int)gyro[0], (int)gyro[1], (int)gyro[2]);
        LOGWARNING("accel: %d %d %d", (int)accel[0], (int)accel[1], (int)accel[2]);
        LOGERROR("temp: %d", (int)temp);
        vTaskDelay(10);
    }
}

