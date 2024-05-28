#ifndef _MONITOR_TASK_H_
#define _MONITOR_TASK_H_

#include "CAN_receive.h"
#include "pid.h"

#define PI 3.14159265358979f
#define rpmCoeff 0.5f

extern bool_t motor_flag;
extern fp32 norm_rad_format(fp32 rad);
extern void motor_data_init(void);
extern void MotorTask(void const *argument);

typedef struct 
{
    /*以下是速度*/
    fp32 desireRpm;     //目标转速
    fp32 realRpm;       //实际转速
    PID_TypeDef pid;    // 速度pid结构体

    /*以下是角度的信息*/
    fp32 desireAngle;       // 目标角度
    fp32 accumAngle;       // 累计转过的角度
    fp32 realAngle;       // 映射到(-pi,pi)的角度
    __int32_t ang_round_num; // 记录转过的圈数
    PID_TypeDef ang_pid;     // 角度pid结构体
    
    fp32 omega;         //rad/s 角速度
} motor_run_data_t; // 电机速度的数据结构

extern void Angle_compute(motor_raw_measure_t *raw_data, motor_run_data_t *motor);

typedef struct
{
    // const RC_ctrl_t *chassis_RC;      // 底盘使用的遥控器指针, the point to remote control
    // const fp32 *chassis_INS_angle;    // the point to the euler angle of gyro sensor.获取陀螺仪解算出的欧拉角指针
    // chassis_mode_e chassis_mode;      // state machine. 底盘控制状态机
    // chassis_mode_e last_chassis_mode; // last state machine.底盘上次控制状态机
    // chassis_motor_t motor_chassis[4]; // chassis motor data.底盘电机数据
    // pid_type_def motor_speed_pid[4];  // motor speed PID.底盘电机速度pid
    // pid_type_def chassis_angle_pid;   // follow angle PID.底盘跟随角度pid

    // first_order_filter_type_t chassis_cmd_slow_set_vx; // use first order filter to slow set-point.使用一阶低通滤波减缓设定值
    // first_order_filter_type_t chassis_cmd_slow_set_vy; // use first order filter to slow set-point.使用一阶低通滤波减缓设定值

    fp32 vx;     // chassis vertical speed, positive means forward,unit m/s. 底盘速度 前进方向 前为正，单位 m/s
    fp32 vy;     // chassis horizontal speed, positive means letf,unit m/s.底盘速度 左右方向 左为正  单位 m/s
    fp32 wz;     // chassis rotation speed, positive means counterclockwise,unit rad/s.底盘旋转角速度，逆时针为正 单位 rad/s
    fp32 vx_set; // chassis set vertical speed,positive means forward,unit m/s.底盘设定速度 前进方向 前为正，单位 m/s
    fp32 vy_set; // chassis set horizontal speed,positive means left,unit m/s.底盘设定速度 左右方向 左为正，单位 m/s
    fp32 wz_set; // chassis set rotation speed,positive means counterclockwise,unit rad/s.底盘设定旋转角速度，逆时针为正 单位 rad/s
    // fp32 chassis_relative_angle;     // the relative angle between chassis and gimbal.底盘与云台的相对角度，单位 rad
    // fp32 chassis_relative_angle_set; // the set relative angle.设置相对云台控制角度
    fp32 chassis_yaw_set;

    fp32 vx_max_speed; // max forward speed, unit m/s.前进方向最大速度 单位m/s
    fp32 vx_min_speed; // max backward speed, unit m/s.后退方向最大速度 单位m/s
    fp32 vy_max_speed; // max letf speed, unit m/s.左方向最大速度 单位m/s
    fp32 vy_min_speed; // max right speed, unit m/s.右方向最大速度 单位m/s
    // fp32 chassis_yaw;   // the yaw angle calculated by gyro sensor and gimbal motor.陀螺仪和云台电机叠加的yaw角度
    // fp32 chassis_pitch; // the pitch angle calculated by gyro sensor and gimbal motor.陀螺仪和云台电机叠加的pitch角度
    // fp32 chassis_roll;  // the roll angle calculated by gyro sensor and gimbal motor.陀螺仪和云台电机叠加的roll角度

} chassis_move_t;

void chasisvxzy_to_desireRpm(void);

#endif