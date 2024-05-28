#include "cmsis_os.h"
#include "MotorTask.h"
#include "main.h"
#include "pid.h"
#include "CAN_receive.h"
#include "remote.h"
#include "ares_protocol.h"
#include "ins_task.h"
#include "usbd_def.h"


motor_run_data_t motor_3508[4]; // 电机驱动电机运动的数据
chassis_move_t chassis_vxyz;

extern rc_protocol_struct usb_rx;
extern INS_t INS;

extern USBD_HandleTypeDef hUsbDeviceFS;
extern uint8_t sbus_connect_flag;

// static uint8_t
// static uint16_t last_totol_yaw = 0;
// static uint16_t num_circle = 0;
// uint8_t motor_flag = 1;
// extern void sbus_to_rpm(void);

/**
 * @brief          电机转动任务，freertos种调用
 * @param[in]      argument:不需要传参，canReceive任务传入的参数
 * @retval         none
 */
void MotorTask(void const *argument)
{
    vTaskDelay(100); // delay一下，让canReceive里面的数据先解算
    // motor_3508[0].ang_pid.PID_reset(&motor_3508[0].ang_pid, 0.08, 0, 2);
    // motor_3508[0].pid.PID_reset(&motor_3508[0].pid, 6, 0.01, 0.1);
    // 防止开始前已经有can累计
    // motor_3508[0].accumAngle = 0;
    motor_3508[0].desireRpm = 0;
    motor_3508[1].desireRpm = 0;
    motor_3508[2].desireRpm = 0;
    while (1)
    {
        // ——————————————
        // 直接遥控器部分
        sbus_to_chasisvxyz();      //遥控器的接收
        chasisvxzy_to_desireRpm(); //底盘运动学解算
        
        // motor_3508[0].desireRpm = 2000;
        // motor_3508[1].desireRpm = 2000;
        // motor_3508[2].desireRpm = 2000;

        for (int i = 0; i < 3; i++)
        {
            PID_Calculate(&motor_3508[i].pid, motor_3508[i].realRpm, motor_3508[i].desireRpm);
        }

        if (sbus_connect_flag){
            CAN_cmd_chassis(motor_3508[0].pid.Output, motor_3508[1].pid.Output, motor_3508[2].pid.Output,0);
        }
        



        // ---------------------------
        // --------------------------


        //上位机部分 
        // if ( (&hUsbDeviceFS)->dev_state == USBD_STATE_CONFIGURED)  // 检测USB是否正常连接
        // {
        //     //四轮全向轮 上位机 usb
        //     motor_3508[0].desireRpm = usb_rx.chasis_motor1;
        //     motor_3508[1].desireRpm = usb_rx.chasis_motor2;
        //     motor_3508[2].desireRpm = usb_rx.chasis_motor3;
        //     motor_3508[3].desireRpm = usb_rx.chasis_motor4;

        //     for (int i = 0; i < 4; i++)
        //     {
        //         PID_Calculate(&motor_3508[i].pid, motor_3508[i].realRpm, motor_3508[i].desireRpm);
        //     }

        //     // if (!motor_flag){
        //     //     motor_3508[1].pid.Output = 0;
        //     //     motor_3508[0].pid.Output = 0;
        //     //     motor_3508[2].pid.Output = 0;
        //     //     motor_3508[3].pid.Output = 0;
        //     // }

        //     CAN_cmd_chassis(motor_3508[0].pid.Output, motor_3508[1].pid.Output, motor_3508[2].pid.Output, motor_3508[3].pid.Output);
        // }
        // else{
        //     motor_3508[0].desireRpm = 0;
        //     motor_3508[1].desireRpm = 0;
        //     motor_3508[2].desireRpm = 0;
        //     motor_3508[3].desireRpm = 0;
        // }
        vTaskDelay(2);

    }
}

/**
 * @brief          所有电机数据初始化，需要选择位置PID或者差分PID
 * @retval         none
 */
void motor_data_init(void)
{
    // 给电机3000的目标转速
    PID_Init_Config chasis_3508_config = {
        .maxOut = 9600,
        .integralLimit = 3000,
        .deadband = 3,
        .kp = 6,
        .ki = 0,
        .kd = 0.1,
        .A = 100,
        .B = 100,
        .output_filtering_coefficient = 0.02,
        .derivative_filtering_coefficient = 0.02,
        .improve = NONE};
    for (int i = 0; i < 4; i++)
    {
        motor_3508[i].desireRpm = motor_3508[i].realRpm = motor_3508[i].desireAngle = 0;
    }

    for (int i = 0; i < 4; i++)
    {
        // PID_Init(&motor_3508[i].ang_pid, 9600, 3000, 0.03, 1, 0, 0, 100, 100, 0.02, 0.02, NONE);
        PID_Init(&motor_3508[i].pid, &chasis_3508_config);
    }
}

void chasisvxzy_to_desireRpm(void)
{
    //四轮麦轮底盘
    // motor_3508[0].desireRpm = (-chassis_vxyz.vx + chassis_vxyz.vy + chassis_vxyz.wz) * rpmCoeff;
    // motor_3508[1].desireRpm = -(chassis_vxyz.vx + chassis_vxyz.vy - chassis_vxyz.wz) * rpmCoeff;
    // motor_3508[2].desireRpm = -(-chassis_vxyz.vx + chassis_vxyz.vy - chassis_vxyz.wz) * rpmCoeff;
    // motor_3508[3].desireRpm = (chassis_vxyz.vx + chassis_vxyz.vy + chassis_vxyz.wz) * rpmCoeff;

    //这是三轮全向轮底盘
    // 0对应左上角motor
    // 1对应右上角motor
    // 2对应的是中心motor 
    motor_3508[0].desireRpm = (chassis_vxyz.vx + chassis_vxyz.vy + chassis_vxyz.wz) * rpmCoeff;
    motor_3508[1].desireRpm = (-chassis_vxyz.vx - chassis_vxyz.vy + chassis_vxyz.wz) * rpmCoeff;
    motor_3508[2].desireRpm = (-1.414 *chassis_vxyz.vx + chassis_vxyz.wz) * rpmCoeff;
}

/**
 * @brief          电机角度解算，包括角度和角速度
 * @param[in]      raw_data: 电机原始数据
 * @param[in]      motor_run: 电机运动数据
 * @retval         none

*/
void Angle_compute(motor_raw_measure_t *raw_data, motor_run_data_t *motor_run)
{
    __uint16_t delta_ecd = raw_data->ecd - raw_data->last_ecd;
    // ecd的测量值为(-4096,4096)，根据电机模型，前后差值的超过4096的话，其通过一次ecd为0的点，圈数可以+1
    if (delta_ecd > 4096)
        motor_run->accumAngle += 2 * PI;
    else if (delta_ecd < -4096)
        motor_run->accumAngle -= 2 * PI;

    // 1. 解算角度:（当前读取码盘值-初始的码盘值+圈数*8192）/ 8192 / 转速比 *2*PI
    //        2. 映射到（-pi，pi）区间
    motor_run->accumAngle += (float)(delta_ecd) / 8192 / 19.02f * 2 * PI;
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
