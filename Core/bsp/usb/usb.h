/**
 * @file usb.h
 * @brief 提供对usb vpc(virtal com port)的操作接口,用于接收函数
 *
 */

#ifndef USB_H
#define USB_H

#pragma once
#include "usbd_cdc_if.h"

static uint8_t *bsp_usb_rx_buffer;

void USBInit(USB_Init_Config_s usb_conf);

typedef struct
{
    USBCallback tx_callback;
    USBCallback rx_callback;
} USB_Init_Config_s;

#endif