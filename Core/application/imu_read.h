#ifndef IMU_READ_H_
#define IMU_READ_H_

#define CALLIBRATE_TIMES 6000;

extern void Imu_read_Task(void const *argument);
extern void calibrate_BMI088(void);
extern void init_calibrate_BMI088(void);

#endif 
