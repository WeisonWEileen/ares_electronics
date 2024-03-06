#include "bsp_imu_pwm.h"
#include "main.h"
#include "tim.h"

void imu_pwm_set(uint16_t pwm)
{
    __HAL_TIM_SetCompare(&htim10, TIM_CHANNEL_1, pwm);
}
