#ifndef _MONITOR_TASK_H_
#define _MONITOR_TASK_H_

#include "CAN_receive.h"
#include "pid.h"

#define PI 3.14159265358979f
extern motor_measure_t motor1;
extern bool_t motor_flag;
extern fp32 norm_rad_format(fp32 rad);
extern void motor_data_init(void);
extern void MotorTask(void const *argument);

typedef struct
{
    fp32 desireRpm;     //目标转速
    fp32 realRpm;       //实际转速
    fp32 given_current; //应该输入的电流
    pid_type_def pid; //速度pid结构体
} motor_v_data_t; // 电机速度的数据结构

typedef struct
{
    __int32_t ang_round_num; //记录转过的圈数
    fp32 relative_angle;     //累计角度
    fp32 relative_speed;     //速度，用于差分计算
    __uint16_t offset_ecd;   //电机上电的时候的码盘值(偏置值)
    fp32 target_angle;       //目标角度
    pid_type_def ang_pid;    //pid结构体
    fp32 given_current;      //给定电流
} motor_ang_data_t;          // 电机角度的数据结构

#endif