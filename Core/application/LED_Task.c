#include <LED_Task.h>
#include "main.h"
#include "cmsis_os.h"
#include "CAN_receive.h"

void LED_Blue_Task(void const *argument)
{
    while (1)
    {
        // CAN_cmd_chassis(2000, 0, 0, 0); debug用
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_10, GPIO_PIN_SET); // 点红灯
        vTaskDelay(1000);
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_10, GPIO_PIN_RESET);
        vTaskDelay(500);
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_SET); // 点红灯
        vTaskDelay(1000);
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_RESET);
        vTaskDelay(500);
    }
}
/**
 * @brief  Key_Interrupt to light blue and green light simultaneously
 * @param  None
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == KEY_interrupt_Pin) // 按一次按键，motor_flag置1，开始一个pi/2的任务
    {
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_SET); // 点红灯
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_10, GPIO_PIN_SET); // 点红灯
    }
}
void LED_Green_Task(void const *argument)
{
    while (1)
    {
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_10, GPIO_PIN_SET); // 点红灯
        vTaskDelay(4000);
        HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_RESET);
        vTaskDelay(1000);
        // HAL_GPIO_WritePin(GPIOH, GPIO_PIN_10, GPIO_PIN_RESET); // 点蓝灯
        // HAL_Delay(1000);
        // HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_RESET); // 点绿灯
        // HAL_Delay(1000);
    }
}