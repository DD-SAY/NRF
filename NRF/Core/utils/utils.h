#pragma once

#include "main.h"

//#include "adc/adc_val.h"
//#include "ano/ano.h"
//#include "encoder/encoder.h"
//#include "iic/soft_iic.h"
//#include "imu/mpu6050.h"
//#include "imu/icm20602.h"
//#include "imu/imu.h"
//#include "imu/imu901.h"
//#include "imu/dmp/inv_mpu.h"
//#include "key/key.h"
//#include "led/led.h"
//#include "motor/dc_motor.h"
//#include "motor/steering_gear.h"
//#include "oled/oled.h"
//#include "pid/pid.h"
//#include "tft/tft.h"

//------------------------------------------ 微秒延时 ------------------------------------------------//
// 用于微秒延时的定时器 将其分频为1M(1us)
#define DELAY_TIM htim7
void delay_us(uint16_t us);
//------------------------------------------ 微秒延时 ------------------------------------------------//


//----------------------------------------- 串口重定向 -----------------------------------------------//
// 要重定向的串口
#define PRINTF_UART huart1
//----------------------------------------- 串口重定向 -----------------------------------------------//


#define limit(val, max, min) do { \
                               if (*(val) > (max)) { *(val) = (max); } \
                               else if (*(val) < (min)) { *(val) = (min); } \
                             } while(0);

#define abs(val) ((val) > 0 ? (val) : -(val))
