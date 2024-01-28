#ifndef PWM_H
#define PWM_H

#include <stdint.h>

void pwm_init(void);
void PWM_led_Task(void const *argument);
void PWM_buzzer_Task(void const *argument);

#endif