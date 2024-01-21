#include "cmsis_os.h"
#include "MotorTask.h"
#include "main.h"
#include "pid.h"
#include "CAN_receive.h"

motor_v_data_t motor_3508[2];   // 电机目标pid调控的数据


/**
 * @brief          电机转动任务，freertos种调用
 * @param[in]      argument:不需要传参，canReceive任务传入的参数
 * @retval         none
 */
void MotorTask(void const *argument)
{
    vTaskDelay(200); //delay一下，让canReceive里面的数据先解算
    motor_data_init();
    while(1){
        motor_3508[0].given_current = PID_calc(&motor_3508[0].pid, motor_3508[0].realRpm, motor_3508[0].desireRpm);
        motor_3508[1].given_current = PID_calc(&motor_3508[1].pid, motor_3508[1].realRpm, motor_3508[1].desireRpm);
        CAN_cmd_chassis(motor_3508[0].given_current, motor_3508[1].given_current, 0, 0);
        vTaskDelay(1);
    }
}

/**
 * @brief          所有电机数据初始化，差分模式
 * @retval         none
 */
void motor_data_init(void)
{
    // 给电机3000的目标转速
    motor_3508[0].desireRpm =  3000;  
    motor_3508[0].realRpm = 0;
    motor_3508[1].desireRpm = -3000;
    motor_3508[1].realRpm = 0;

    const fp32 pos_pid_3508[3] = {0.5, 0.5, 0.5};
    PID_init(&motor_3508[0].pid, PID_DELTA , pos_pid_3508, 16000, 200); // 初始化pid为差分PID模式
    PID_init(&motor_3508[1].pid, PID_DELTA , pos_pid_3508, 16000, 200); // 初始化pid为差分PID模式

}



fp32 norm_rad_format(fp32 angle)
{
    if (angle > PI)
    {
        while (angle > PI)
        {
            angle -= 2 * PI;
        }
    }
    else if (angle < -PI)
    {
        while (angle < -PI)
        {
            angle += 2 * PI;
        }
    }
    return angle;
}

// 防止频繁按键
// bool_t motor_flag = 1;
// int key_delay = 1000;

// void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
// {
//     if (GPIO_Pin == KEY_interrupt_Pin) // 按一次按键，motor_flag置1，开始一个pi/2的任务
//     {
//         if (key_delay == 0)
//         {
//             motor_flag = 1;
//             key_delay = 1000;
//         }
//     }
// }

// void MotorTask(void const *argument)
// {
//     vTaskDelay(200);
//     motor_ang_data_t_init();
//     vTaskDelay(200);//delay一下，让数据解算完毕，比如canReceive里面的一堆
//     motor_ang_data_t_init();

//     while (1)
//     {
//         if (key_delay > 0)
//         {
//             key_delay--;
//         }

//         if (motor_flag)
//         {
//             //获取目前的角度值，在基础上加上target值pi/2，每按一次按键新开始一个pi/2的任务
//             motor_3508.target_angle = norm_rad_format(motor_3508.relative_angle + PI / 2);
//             motor_flag = 0;
//         }
//         motor_3508.given_current = PID_calc(&motor_3508.ang_pid, motor_3508.relative_angle, motor_3508.target_angle, motor_3508.relative_speed);
//         Limit_ang_out(&motor_3508.given_current, 16384);
//         CAN_cmd_chassis(motor_3508.given_current, 0, 0, 0);
//         // CAN_cmd_chassis(2000, 0, 0, 0);

//         vTaskDelay(1);
//     }
// }

// void motor_ang_data_t_init(void)
// { // 电机的数据初始化
//     motor_3508.offset_ecd = 0;
//     motor_3508.relative_speed = 0;
//     motor_3508.ang_round_num = 0;
//     motor_3508.target_angle = motor_3508.relative_angle;

//     fp32 pos_pid_3508[3] = {2000, 0, -200};
//     PID_init(&motor_3508.ang_pid, pos_pid_3508, 5000, 500);
// }