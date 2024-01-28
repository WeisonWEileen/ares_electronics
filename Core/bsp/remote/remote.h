#include "struct_typedef.h"
#include "usart.h"


#define SBUS_DATA_SIZE 25 // 25字节
struct SBUS_t
{
    uint16_t ch[16]; // 16个字节数据
};

extern void SBUS_IT_Open(void);
extern void sbus_to_chasisvxyz(void);

#define SBUS0_DEFAULT  1000
#define SBUS1_DEFAULT  1000
#define SBUS2_DEFAULT  1024
#define SBUS3_DEFAULT  1000
#define SBUS4_DEFAULT  1024
#define SBUS5_DEFAULT  1024
#define SBUS6_DEFAULT  1695
#define SBUS7_DEFAULT  353
#define SBUS8_DEFAULT  1695
#define SBUS9_DEFAULT  353
#define SBUS10_DEFAULT 1024
#define SBUS11_DEFAULT 1024
#define SBUS12_DEFAULT 0
#define SBUS13_DEFAULT 0
#define SBUS14_DEFAULT 0
#define SBUS15_DEFAULT 0