#include "remote.h"
#include "MotorTask.h"

uint8_t sbus_rx_sta = 0;             // sbus 接收状态，0：未完成，1：已完成一帧接收
uint8_t sbus_rx_buf[SBUS_DATA_SIZE]; // 接收sbus数据缓冲区
uint8_t connect_flag = 0;              // 连接是否正常
struct SBUS_t sbus;                  // SBUS 结构体实例化

extern motor_run_data_t motor_3508[1];

void SBUS_IT_Open(void)
{
    HAL_UARTEx_ReceiveToIdle_IT(&huart3, (uint8_t *)sbus_rx_buf, SBUS_DATA_SIZE);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART3)
    {
        if ((sbus_rx_buf[0] == 0x0F) && sbus_rx_buf[24] == 0x00) // 校验收到的数据是否，正确帧头：0x0f；正确帧尾：0x00
        {
            sbus_rx_sta = 1;
        }
        else
        {
            sbus_rx_sta = 0;
        }

        if (sbus_rx_sta == 1)
        {
            sbus.ch[0] = ((sbus_rx_buf[2] << 8) + (sbus_rx_buf[1])) & 0x07ff;
            sbus.ch[1] = ((sbus_rx_buf[3] << 5) + (sbus_rx_buf[2] >> 3)) & 0x07ff;
            sbus.ch[2] = ((sbus_rx_buf[5] << 10) + (sbus_rx_buf[4] << 2) + (sbus_rx_buf[3] >> 6)) & 0x07ff;
            sbus.ch[3] = ((sbus_rx_buf[6] << 7) + (sbus_rx_buf[5] >> 1)) & 0x07ff;
            sbus.ch[4] = ((sbus_rx_buf[7] << 4) + (sbus_rx_buf[6] >> 4)) & 0x07ff;
            sbus.ch[5] = ((sbus_rx_buf[9] << 9) + (sbus_rx_buf[8] << 1) + (sbus_rx_buf[7] >> 7)) & 0x07ff;
            sbus.ch[6] = ((sbus_rx_buf[10] << 6) + (sbus_rx_buf[9] >> 2)) & 0x07ff;
            sbus.ch[7] = ((sbus_rx_buf[11] << 3) + (sbus_rx_buf[10] >> 5)) & 0x07ff;
            sbus.ch[8] = ((sbus_rx_buf[13] << 8) + (sbus_rx_buf[12])) & 0x07ff;
            sbus.ch[9] = ((sbus_rx_buf[14] << 5) + (sbus_rx_buf[13] >> 3)) & 0x07ff;
            sbus.ch[10] = ((sbus_rx_buf[16] << 10) + (sbus_rx_buf[15] << 2) + (sbus_rx_buf[14] >> 6)) & 0x07ff;
            sbus.ch[11] = ((sbus_rx_buf[17] << 7) + (sbus_rx_buf[16] >> 1)) & 0x07ff;
            sbus.ch[12] = ((sbus_rx_buf[18] << 4) + (sbus_rx_buf[17] >> 4)) & 0x07ff;
            sbus.ch[13] = ((sbus_rx_buf[20] << 9) + (sbus_rx_buf[19] << 1) + (sbus_rx_buf[18] >> 7)) & 0x07ff;
            sbus.ch[14] = ((sbus_rx_buf[21] << 6) + (sbus_rx_buf[20] >> 2)) & 0x07ff;
            sbus.ch[15] = ((sbus_rx_buf[22] << 3) + (sbus_rx_buf[21] >> 5)) & 0x07ff;




            if (sbus_rx_buf[21] != 0x00)
                connect_flag = 0;
            else
                connect_flag = 1;

            sbus_rx_sta = 0; // 准备下一次接收
        }
        SBUS_IT_Open();
    }
}
void sbus_to_rpm(void)
{
    motor_3508[0].desireRpm = 16*(sbus.ch[1] - SBUS1_DEFAULT);
}