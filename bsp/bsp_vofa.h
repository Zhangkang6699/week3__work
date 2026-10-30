/**
  ******************************************************************************
  * @file    bsp_vofa.h
  * @brief   VOFA+ 上位机的两种数据协议封装
  * @note    JustFloat 走二进制，用来画波形；FireWater 走文本，用来打印数字。
  ******************************************************************************
  */
#ifndef __BSP_VOFA_H
#define __BSP_VOFA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "usart.h"

/**
  * @brief  按 JustFloat 协议发送一组 float，VOFA+ 收到后会画成波形
  * @param  data 数据数组首地址
  * @param  num  元素个数（也就是波形通道数）
  */
void Vofa_JustFloat(float *data, uint8_t num);

/**
  * @brief  按 FireWater 协议发送一行文本
  * @param  fmt 和 printf 一样的格式串
  * @note   注意 newlib-nano 默认不支持 %f，只能打印整数和字符串
  */
void Vofa_FireWater(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_VOFA_H */
