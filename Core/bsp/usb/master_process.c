/**
 * @file master_process.c
 * @author neozng
 * @brief  module for recv&send vision data
 * @version beta
 * @date 2022-11-03
 * @todo 增加对串口调试助手协议的支持,包括vofa和serial debug
 * @copyright Copyright (c) 2022
 *
 */
#include "master_process.h"
#include "ares_protocol.h"
// #include "daemon.h"
#include "bsp_log.h"
// #include "robot_def.h"

// static Chasis_move_cmd recv_data;
static rc_protocol_struct recv_data;
// static Chasis_move_cmd send_data;
// static DaemonInstance *vision_daemon_instance;

// void VisionSetFlag(Enemy_Color_e enemy_color, Work_Mode_e work_mode, Bullet_Speed_e bullet_speed)
// {
//     send_data.enemy_color = enemy_color;
//     send_data.work_mode = work_mode;
//     send_data.bullet_speed = bullet_speed;
// }

// void VisionSetAltitude(float yaw, float pitch, float roll)
// {
//     send_data.yaw = yaw;
//     send_data.pitch = pitch;
//     send_data.roll = roll;
// }

// /**
//  * @brief 离线回调函数,将在daemon.c中被daemon task调用
//  * @attention 由于HAL库的设计问题,串口开启DMA接收之后同时发送有概率出现__HAL_LOCK()导致的死锁,使得无法
//  *            进入接收中断.通过daemon判断数据更新,重新调用服务启动函数以解决此问题.
//  *
//  * @param id vision_usart_instance的地址,此处没用.
//  */
// static void VisionOfflineCallback(void *id)
// {
// #ifdef VISION_USE_UART
//     USARTServiceInit(vision_usart_instance);
// #endif // !VISION_USE_UART
//     LOGWARNING("[vision] vision offline, restart communication.");
// }


// #ifdef VISION_USE_VCP

#include "bsp_usb.h"
static uint8_t *chasis_recv_buff;

static void DecodeChasis(void)
{
    rc_get_protocol_info(chasis_recv_buff, &recv_data);
    // TODO: code to resolve flag_register;
}

/* 视觉通信初始化 */
rc_protocol_struct *Chasis_cmd_Init(UART_HandleTypeDef *_handle)
{
    UNUSED(_handle); // 仅为了消除警告
    USB_Init_Config_s conf = {.rx_cbk = DecodeChasis};
    chasis_recv_buff = USBInit(conf);
    return &recv_data;
}

