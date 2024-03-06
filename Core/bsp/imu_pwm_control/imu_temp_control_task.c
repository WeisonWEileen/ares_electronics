#include "imu_temp_control_task.h"
#include "BMI088driver.h"
#include "BMI088reg.h"
#include "cmsis_os.h"
#include "main.h"
#include "pid.h"
#include "bsp_imu_pwm.h"

#define IMU_temp_PWM(pwm) imu_pwm_set(pwm) // pwm给定

#define TEMPERATURE_PID_KP 1600.0f // kp of temperature control PID
#define TEMPERATURE_PID_KI 0.2f    // ki of temperature control PID
#define TEMPERATURE_PID_KD 0.0f    // kd of temperature control PID

#define TEMPERATURE_PID_MAX_OUT 4500.0f  // max out of temperature control PID
#define TEMPERATURE_PID_MAX_IOUT 4400.0f // max iout of temperature control PID

volatile uint8_t imu_start_flag = 0;

// kp, ki,kd three params
PID_TypeDef imu_temp_pid;

/**
 * @brief          bmi088 temperature control
 * @param[in]      argument: NULL
 * @retval         none
 */
/**
 * @brief          bmi088温度控制
 * @param[in]      argument: NULL
 * @retval         none
 */
void imu_temp_control_task(void const *argument)
{
    osDelay(500);
    // pid init  PID初始化
    PID_Init_Config bim_temp_config = {
        .maxOut = 4500.f,
        .integralLimit = 300.f,
        .deadband = 0.f,
        .kp = 1600.f,
        .ki = 0.2f,
        .kd = 0.f,
        .A = 0.f,
        .B = 0.f,
        .output_filtering_coefficient = 0.02f,
        .derivative_filtering_coefficient = 0.02f,
        .improve = Integral_Limit};
    PID_Init(&imu_temp_pid, &bim_temp_config);

    // bmi088 init. bmi088初始化
    //  while(BMI088_init())
    //  {
    //      ;
    //  }
    // set spi frequency
    //  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;

    // if (HAL_SPI_Init(&hspi1) != HAL_OK)
    // {
    //     Error_Handler();
    // }
    // get task handle, must enable 'xTaskGetHandle' in cubeMX
    // 获取任务句柄，必须在cubeMX使能'xTaskGetHandle'
    while (1)
    {

        // read data.读取数据
        BMI088_Read(&BMI088);

        uint16_t tempPWM;
        // pid calculate. PID计算
        PID_Calculate(&imu_temp_pid, BMI088.Temperature, 40.0f);
        if (imu_temp_pid.Output < 0.0f)
        {
            imu_temp_pid.Output = 0.0f;
        }
        tempPWM = (uint16_t)imu_temp_pid.Output;
        IMU_temp_PWM(tempPWM);


        vTaskDelay(1);
    }
}

void Imu_read_temp_Task(void const *argument)
{
    PID_Init_Config bim_temp_config = {
        .maxOut = 4600,
        .integralLimit = 300,
        .deadband = 0,
        .kp = 1000,
        .ki = 20,
        .kd = 0,
        .A = 0,
        .B = 0,
        // .output_filtering_coefficient = 0.02,
        // .derivative_filtering_coefficient = 0.02,
        .improve = Integral_Limit};
    PID_Init(&imu_temp_pid, &bim_temp_config);
    while (1)
    {
        // TODO 目前还没有解决加速度的偏置，以及深圳所纬度的加速度的测定
        // TODO 以及姿态解算！
        // TODO 目前经常一次正常情况下BMI初始化失败，待分析具体原因，报错call not read ID
        BMI088_Read(&BMI088);

        PID_Calculate(&imu_temp_pid, BMI088.Temperature, 40.0f);
        if (imu_temp_pid.Output < 0.0f)
        {
            imu_temp_pid.Output = 0.0f;
        }
        IMU_temp_PWM((uint16_t)imu_temp_pid.Output);
        vTaskDelay(10);
    }
}
