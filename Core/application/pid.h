/**
  ******************************************************************************
  * @file	 pid.h
  * @author  Hongxi Wong
  * @version V1.0.6
  * @date    2019/12/17
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef _PID_H
#define _PID_H

#include <stdint.h>
#include "struct_typedef.h"

#define ABS(x) ((x > 0) ? x : -x)

typedef enum pid_Improvement_e
{
    NONE = 0X00,                        //0000 0000
    Integral_Limit = 0x01,              //0000 0001
    Derivative_On_Measurement = 0x02,   //0000 0010
    Trapezoid_Intergral = 0x04,         //0000 0100
    Proportional_On_Measurement = 0x08, //0000 1000
    OutputFilter = 0x10,                //0001 0000
    ChangingIntegralRate = 0x20,        //0010 0000
    DerivativeFilter = 0x40,            //0100 0000
    ErrorHandle = 0x80,                 //1000 0000
} PID_Improvement_e;

typedef enum errorType_e
{
    PID_ERROR_NONE = 0x00U,
    Motor_Blocked = 0x01U
} ErrorType_e;

typedef struct
{
    uint64_t ERRORCount;
    ErrorType_e ERRORType;
} PID_ErrorHandler_t;

typedef struct _PID_TypeDef
{
    fp32 Target;
    fp32 LastNoneZeroTarget;
    fp32 Kp;
    fp32 Ki;
    fp32 Kd;

    fp32 Measure;
    fp32 Last_Measure;
    fp32 Err;
    fp32 Last_Err;

    fp32 Pout;
    fp32 Iout;
    fp32 Dout;
    fp32 ITerm;

    fp32 Output;
    fp32 Last_Output;
    fp32 Last_Dout;

    fp32 MaxOut;
    fp32 IntegralLimit;
    fp32 DeadBand;
    fp32 ControlPeriod;
    fp32 MaxErr;
    fp32 ScalarA; //For Changing Integral
    fp32 ScalarB; //ITerm = Err*((A-abs(err)+B)/A)  when B<|err|<A+B
    fp32 Output_Filtering_Coefficient;
    fp32 Derivative_Filtering_Coefficient;

    uint32_t thistime;
    uint32_t lasttime;
    uint8_t dtime;

    uint8_t Improve;

    PID_ErrorHandler_t ERRORHandler;

    void (*PID_param_init)(
        struct _PID_TypeDef *pid,
        uint16_t maxOut,
        uint16_t integralLimit,
        fp32 deadband,
        fp32 Kp,
        fp32 ki,
        fp32 kd,
        fp32 A,
        fp32 B,
        fp32 output_filtering_coefficient,
        fp32 derivative_filtering_coefficient,
        uint8_t improve);

    void (*PID_reset)(
        struct _PID_TypeDef *pid,
        fp32 Kp,
        fp32 ki,
        fp32 kd);
} PID_TypeDef;

extern void f_Trapezoid_Intergral(PID_TypeDef *pid);
extern void f_Integral_Limit(PID_TypeDef *pid);
extern void f_Derivative_On_Measurement(PID_TypeDef *pid);
extern void f_Changing_Integral_Rate(PID_TypeDef *pid);
extern void f_Output_Filter(PID_TypeDef *pid);
extern void f_Derivative_Filter(PID_TypeDef *pid);
extern void f_Output_Limit(PID_TypeDef *pid);
extern void f_Proportion_Limit(PID_TypeDef *pid);
extern void f_PID_ErrorHandle(PID_TypeDef *pid);

void PID_Init(
    PID_TypeDef *pid,
    uint16_t max_out,
    uint16_t intergral_limit,
    fp32 deadband,

    fp32 kp,
    fp32 ki,
    fp32 kd,

    fp32 A,
    fp32 B,

    fp32 output_filtering_coefficient,
    fp32 derivative_filtering_coefficient,

    uint8_t improve);
fp32 PID_Calculate(PID_TypeDef *pid, fp32 measure, fp32 target);
		
#endif