#ifndef MASTER_PROCESS_H
#define MASTER_PROCESS_H

// #include "bsp_usart.h"
#include "ares_protocol.h"

//内存对齐，提高接收效率
#pragma pack(1)
// 上位机发过来的底盘移动指令
typedef struct
{
	uint8_t forward_back;
	uint8_t left_right;
} Chasis_move_cmd;

#pragma pack()

// /**
//  * @brief 调用此函数初始化和上位机的虚拟串口通信
//  */

// extern Chasis_move_cmd *Chasis_cmd_Init(UART_HandleTypeDef *_handle);



#endif // !MASTER_PROCESS_H