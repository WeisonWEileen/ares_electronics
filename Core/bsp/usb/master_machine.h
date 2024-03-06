#ifndef MASTER_MACHINE_H
#define MASTER_MACHINE_H

#include "struct_typedef.h"
#include "usart.h"

typedef struct
{
    uint8_t row;
    uint8_t yaw;
} IMU_Receive;

/**
 * @brief IMU通信初始化
 * @param handle 用于和IMU通信的串口的handle(c板上一般是USART1)
 */
IMU_Receive *IMU_Init(UART_HandleTypeDef *_handle);

static void DecodeIMU(uint16_t rec);

typedef struct
{
    struct
    {
        uint8_t sof;
        uint16_t data_length;
        uint8_t crc_check; // 帧头CRC校验
    } header;              // 数据帧头
    uint16_t cmd_id;       // 数据ID
    uint16_t frame_tail;   // 帧尾CRC校验1
} protocol_rm_struct;

/*更新发送数据帧，并计算发送数据帧长度*/
void get_protocol_send_data(uint16_t send_id,        // 信号id
                            uint16_t flags_register, // 16位寄存器
                            float *tx_data,          // 待发送的float数据
                            uint8_t float_length,    // float的数据长度
                            uint8_t *tx_buf,         // 待发送的数据帧
                            uint16_t *tx_buf_len);   // 待发送的数据帧长度

/*接收数据处理*/
uint16_t get_protocol_info(uint8_t *rx_buf,          // 接收到的原始数据
                           uint16_t *flags_register, // 接收数据的16位寄存器地址
                           uint8_t *rx_data);        // 接收的float数据存储地址

#endif
