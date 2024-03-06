/**
 * @brief module for receive and send imu data
 * @date 2024-3-3
 * @author weison_pan
 */

#include "master_machine.h"

static void Decode_Chasis(uint16_t recv_len){
    get_protocol_info
    }

IMU_Receive *Decode_Chasis_Init(UART_HandleTypeDef *_handle)
{
    UNUSED(_handle); // 仅为了消除警告
}

/*
    此函数用于处理接收数据，
    返回数据内容的id
*/
uint16_t get_protocol_info(uint8_t *rx_buf,          // 接收到的原始数据
                           uint16_t *flags_register, // 接收数据的16位寄存器地址
                           uint8_t *rx_data)         // 接收的float数据存储地址
{
    // 放在静态区,避免反复申请栈上空间
    static protocol_rm_struct pro;
    static uint16_t date_length;

    if (protocol_heade_Check(&pro, rx_buf))
    {
        date_length = OFFSET_BYTE + pro.header.data_length;
        if (CRC16_Check_Sum(&rx_buf[0], date_length))
        {
            *flags_register = (rx_buf[7] << 8) | rx_buf[6];
            memcpy(rx_data, rx_buf + 8, pro.header.data_length - 2);
            return pro.cmd_id;
        }
    }
    return 0;
}

// static uint8_t *imu_receive_buffer;
// /**
//  * @brief 解码获取到的imu信息，并且储存到imu_receive_buffer中
//  */
// static void DecodeIMU(uint16_t rec)
// {
//     get_protocol_info(imu_receive_buffer，)
// }