#ifndef ROBOT_H
#define ROBOT_H

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "robot.h"
#include "ins_task.h"
#include "master_process.h"
#include "bsp_log.h"

void StartINSTASK(void const *argument);

#endif