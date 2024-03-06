# 这里有基本的BB响的功能，还有亮灯的功能
## 关于RGB灯
cubeMX中我设置了TIM5 的 counter_Period 为65535,可通过 `HAL_TIM_PWM_Start`开启RGB的三个`TIM_CHANNEL`，通过`__HAL_TIM_SET_COMPARE`设置pwm的占空比，即设置电压值，$U_0=VCC*\frac{t}{T}=VCC*duty$，从而设置rgb的亮度


## 关于buzzer
通过`__HAL_TIM_PRESCALER`设置音调,通过`__HAL_TIM_SET_COMPARE`设置音量

---


注意 : 
- 目前我并没有用到定时器相关的中断操作,也就是目前还没有使用`HAL_BASE_START_IT`；
- `__HAL`开头的宏函数往往比较偏底层，倾向于直接对寄存器进行操作