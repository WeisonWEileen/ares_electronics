#include "struct_typedef.h"
#include "usart.h"


#define SBUS_DATA_SIZE 25 // 25字节
struct SBUS_t
{
    uint16_t ch[16]; // 16个字节数据
};

extern void SBUS_IT_Open(void);