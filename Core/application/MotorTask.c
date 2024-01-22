#include "cmsis_os.h"
#include "MotorTask.h"
#include "main.h"
#include "pid.h"
#include "CAN_receive.h"

motor_run_data_t motor_3508[2]; // 电机驱动电机运动的数据

/**
 * @brief          电机转动任务，freertos种调用
 * @param[in]      argument:不需要传参，canReceive任务传入的参数
 * @retval         none
 */
void MotorTask(void const *argument)
{
    vTaskDelay(100); //delay一下，让canReceive里面的数据先解算
    motor_data_init();
    while(1){
        PID_Calculate(&motor_3508[0].pid, motor_3508[0].realRpm, motor_3508[0].desireRpm);
        CAN_cmd_chassis(motor_3508[0].pid.Output, 0, 0, 0);
        vTaskDelay(1);
    }
}

/**
 * @brief          所有电机数据初始化，需要选择位置PID或者差分PID
 * @retval         none
 */
void motor_data_init(void)
{
    // 给电机3000的目标转速
    motor_3508[0].desireRpm =  3000;  
    motor_3508[0].realRpm = 0;
    // motor_3508[0].desireAngle = PI/2;
    // motor_3508[1].desireRpm = -3000;
    // motor_3508[1].realRpm = 0;

    for(int i=0; i<sizeof(motor_3508)/sizeof(motor_3508[0]); i++){
        PID_Init(&motor_3508[i].pid, 9600, 3000,3,  5,0.2,0,   100,100,  0.02,0.02, (ErrorHandle | Integral_Limit | OutputFilter));
    }
}
/**
 * @brief          电机角度解算，包括角度和角速度
 * @param[in]      raw_data: 电机原始数据
 * @param[in]      motor_run: 电机运动数据
 * @retval         none
 
*/
void Angle_compute(motor_raw_measure_t *raw_data, motor_run_data_t *motor_run)
{
    // 下面是调角度的东西
    if ((raw_data->ecd - raw_data->last_ecd) > 4096)
        motor_run->ang_round_num++;
    else if ((raw_data->ecd - raw_data->last_ecd) < -4096)
        motor_run->ang_round_num++;

    // 1. 解算角度:（当前读取码盘值-初始的码盘值+圈数*8192）/ 8192 / 转速比 *2*PI
    // 2. 映射到（-pi，pi）区间
    motor_run->relative_angle = norm_rad_format((float)(raw_data->ecd - motor_run->offset_ecd + motor_run->ang_round_num * 8192) / 8192 / 19.02f * 2 * PI);

    // 查电调手册可知，relative_speed是转子的转速值，rpm
    // 但是我们需要的数据是rad/s 
    motor_run->omega = (float)raw_data->speed_rpm * 60 * 2 * PI / 19.02f;
    //目前还不知道这里是否冗余，因为realRpm那里我已经读了用了一次speed_rpm了
    // motor_run->relative_speed = (float)motor1.speed_rpm / 8192 * 2 * PI / 19.02f;（这里目前是错的，但是教程说和后面的代码有关，我先不改）

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