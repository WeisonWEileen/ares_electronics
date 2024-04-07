#ifndef __SEASKY_PROTOCOL_H
#define __SEASKY_PROTOCOL_H

#include <stdio.h>
#include <stdint.h>

#define PROTOCOL_CMD_ID 0X5A // the master machine to the MCU is 0X5A
#define OFFSET_BYTE 8		 // 除数据段外，其他部分所占字节数
#define RC_PROTOCO_DATA_LEGNTH 19 // 整个数据帧长度


#define EXTRACT_CRC16(low,high) //便于提取16位校验码

// typedef struct
// {
// 	struct
// 	{
// 		uint8_t sof;
// 		uint16_t data_length;
// 		uint8_t crc_check; // 帧头CRC校验
// 	} header;			   // 数据帧头
// 	uint16_t cmd_id;	   // 数据ID
// 	uint16_t frame_tail;   // 帧尾CRC校验1
// } protocol_rm_struct;


//注意，gcc-arm-none-eabi不支持__packed__关键字
#pragma pack(1)
typedef struct
{
	uint8_t header;
	float chasis_motor1;
	float chasis_motor2;
	float chasis_motor3;
	float chasis_motor4;
	uint16_t checksum;
} rc_protocol_struct ;
#pragma pack()

/*更新发送数据帧，并计算发送数据帧长度*/
void
get_protocol_send_data(uint16_t send_id,		// 信号id
					   uint16_t flags_register, // 16位寄存器
					   float *tx_data,			// 待发送的float数据
					   uint8_t float_length,	// float的数据长度
					   uint8_t *tx_buf,			// 待发送的数据帧
					   uint16_t *tx_buf_len);	// 待发送的数据帧长度

/*接收数据处理*/
uint16_t get_protocol_info(uint8_t *rx_buf,			 // 接收到的原始数据
						   uint16_t *flags_register, // 接收数据的16位寄存器地址
						   uint8_t *rx_data);		 // 接收的float数据存储地址

void rc_get_protocol_info(uint8_t *rx_buf, uint8_t *rx_data);

// static uint8_t rc_check_head_crcsum(rc_protocol_struct *pro, uint8_t *rx_buf);




#endif
