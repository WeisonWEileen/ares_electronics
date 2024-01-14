#include "main.h"
#include "UsartTask.h"
#include "cmsis_os.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "MotorTask.h"
// uint8_t buf[] = "helloworld\r\n";//发送字符串
char Buf[10]; //发送数字
uint8_t num = 0;

void usart_Task()
{
    while(1){
        if (motor_flag){
        sprintf(Buf, "%d" , num);   //把一个整型直接赋给字符串
        HAL_UART_Transmit(&huart1, (uint8_t*)Buf, strlen(Buf), 1000);
        num++;
        vTaskDelay(500);
        }
    }
}