#include "usb.h"
#include "bsp_log.h"


uint8_t *USBInit(USB_Init_Config_s usb_conf)
{
    bsp_usb_rx_buffer = CDCInitRxbufferNcallback(usb_conf.tx_callback, usb_conf.rx_callback); // 获取接收数据指针
    // usb的接收回调函数会在这里被设置,并将数据保存在bsp_usb_rx_buffer中
    LOGINFO("USB init success");
    return bsp_usb_rx_buffer;
}
