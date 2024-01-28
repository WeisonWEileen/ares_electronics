#include "bsp_log.h"

#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"
#include <stdio.h>


void BSPLogInit()
{
    SEGGER_RTT_Init();
    SEGGER_RTT_ConfigUpBuffer(0, "SEGGER RTT", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_TRIM);
    SEGGER_RTT_ConfigDownBuffer(0, "SEGGER RTT", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_TRIM);
    SEGGER_RTT_ConfigUpBuffer(1, "Graph", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_TRIM);
    SEGGER_RTT_ConfigDownBuffer(1, "Graph", NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_TRIM);
    LOG("!!!!!!! Log subsystem initialized !!!!!!!!!!");
    LOGINFO("           ___    ____  ___________");
    LOGINFO("          /   |  / __ \\/ ____/ ___/");
    LOGINFO("         / /| | / /_/ / __/  \\__ \\ ");
    LOGINFO("        / ___ |/ _, _/ /___ ___/ / ");
    LOGINFO("       /_/  |_/_/ |_/_____//____/  ");
    LOGWARNING("Association of Robotics Engineers in SUSTech");
    LOGERROR("weison_pan go ares go");
}

int PrintLog(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int n = SEGGER_RTT_vprintf(BUFFER_INDEX, fmt, &args); // 一次可以开启多个buffer(多个终端),我们只用一个
    va_end(args);
    return n;
}

void Float2Str(char *str, float va)
{
    int flag = va < 0;
    int head = (int)va;
    int point = (int)((va - head) * 1000);
    head = abs(head);
    point = abs(point);
    if (flag)
        sprintf(str, "-%d.%d", head, point);
    else
        sprintf(str, "%d.%d", head, point);
}

