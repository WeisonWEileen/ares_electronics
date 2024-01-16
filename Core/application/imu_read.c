#include "cmsis_os.h"
#include "BMI088driver.h"
#include "main.h"
//#include "spi.h"
//#include "gpio.h"


fp32 gyro[3], accel[3], temp;

/**
* @brief          IMU读取任务，写入到gyro, accel, temp中
* @retval         none
*/
void Imu_read_Task(void const *argument)
{
    while (1)
    {
        BMI088_read(gyro, accel, &temp);   
        vTaskDelay(10);
    }
}

