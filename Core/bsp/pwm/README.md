# 这里有基本的BB响的功能，还有亮灯的功能
## 关于RGB灯
cubeMX中我设置了TIM5 的 counter_Period 为65535,可通过 `HAL_TIM_PWM_Start`设置三个`TIM_CHANNEL`的值即可设置RGB三个通道的分量，通过`__HAL_TIM_SET_COMPARE`设置pwm的占空比，从而设置rgb的亮度


## 关于buzzer
通过`__HAL_TIM_PRESCALER`设置音调,通过`__HAL_TIM_SET_COMPARE`设置音量