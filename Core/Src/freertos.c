/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MotorTask.h"
#include "UsartTask.h"
#include "bsp_log.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
osThreadId led_blue_TaskHandle;
osThreadId motor_run_TaskHandle;
osThreadId imu_read_write_TaskHandle;
osThreadId adc_sample_TaskHandle;
osThreadId pwm_led_TaskHandle;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
__weak void LED_Blue_Task(void const *argument);
__weak void MotorTask(void const *argument);
__weak void Imu_read_temp_Task(void const *argument);
__weak void ADC_sample_Task(void const *argument);
__weak void PWM_led_Task(void const *argument);
__weak void PWM_buzzer_Task(void const *argument);

// extern void Imu_read_temp_Task(void const *argument);
// extern void MotorTask(void const *argument);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  // osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  // defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of LED_Task_Blue */
  // osThreadDef(LED_Task_Blue, LED_Blue_Task, osPriorityNormal, 0, 128);
  // led_blue_TaskHandle = osThreadCreate(osThread(LED_Task_Blue), NULL);

  /* definition and creation of motor_ang_task */
  osThreadDef(motor_ang_task, MotorTask, osPriorityAboveNormal, 0, 128);
  motor_run_TaskHandle = osThreadCreate(osThread(motor_ang_task), NULL);

  /* definition and creation of imu_read_Task */
  osThreadDef(imu_read_temp_Task, Imu_read_temp_Task, osPriorityHigh, 0, 128 * 4);
  imu_read_write_TaskHandle = osThreadCreate(osThread(imu_read_temp_Task), NULL);

  /* definition and creation of imu_read_Task */
  // osThreadDef(imu_read_Task, Imu_read_temp_Task, osPriorityAboveNormal, 0, 128);
  // imu_read_TaskHandle = osThreadCreate(osThread(imu_read_Task), NULL);

  /* definition and creation of ADC_sample_Task */
  // osThreadDef(adc_sample_Task, ADC_sample_Task, osPriorityNormal, 0, 128);
  // adc_sample_TaskHandle = osThreadCreate(osThread(adc_sample_Task), NULL);

  /* definition and creation of LED_PWM_Task */
  osThreadDef(pwm_led, PWM_led_Task, osPriorityAboveNormal, 0, 128);
  pwm_led_TaskHandle = osThreadCreate(osThread(pwm_led), NULL);

  /* definition and creation of LED_PWM_Task */
  // osThreadDef(pwm_buzzer, PWM_buzzer_Task, osPriorityNormal, 0, 128);
  // pwm_led_TaskHandle = osThreadCreate(osThread(pwm_buzzer), NULL);

  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
  osThreadTerminate(NULL);
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
__weak void LED_Blue_Task(void const *argument)
{
  for (;;)
  {
    osDelay(1);
  }
}
__weak void MotorTask(void const *argument)
{
  for (;;)
  {
    osDelay(1);
  }
}

__weak void Imu_read_temp_Task(void const *argument)
{
  for (;;)
  {
    osDelay(1);
  }
}
__weak void ADC_sample_Task(void const *argument)
{
  for (;;)
  {
    osDelay(1);
  }
}
__weak void PWM_led_Task(void const *argument)
{
  for (;;)
  {
    osDelay(1);
  }
}
__weak void PWM_buzzer_Task(void const *argument)
{
  for (;;)
  {
    osDelay(1);
  }
}

/* USER CODE END Application */
