#include "pwm.h"
#include "tim.h"
#include "bsp_log.h"
#include "cmsis_os.h"
#include "bsp_dwt.h"
#include "cmsis_os.h"

extern TIM_HandleTypeDef htim5;
uint8_t counter = 0;
uint32_t psc=0,state=0;    

void pwm_init(void)
{
    HAL_TIM_Base_Start(&htim5);           //开启定时器,为什么是中断模式?
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1); //rgb的三个灯的PWM产生命令
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3);
    // HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
}

/*
 * @brief: set pwm value
 * @param: pwm1, pwm2, pwm3 (0-65535)
 * @retval: None
 */
void PWM_led_Task(void const *argument)
{
    while (1)
    {
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, 10000);
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, 10000);
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, 10000);
        osDelay(1000);
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, 0);
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, 0);
        osDelay(1000);
    }
}

/*
 * @brief: give warning sound
 * @param: None
 * @retval: None
 */
void PWM_buzzer_Task(void const *argument)
{
    while(1){
        __HAL_TIM_PRESCALER(&htim4, 1);                      //这是设置音调
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 10000); //这是调节设置响度
        osDelay(1000);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
        osDelay(1000);
    }
}

//TODO 融入到初始化失败中,buzzer响