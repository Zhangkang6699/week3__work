/**
  ******************************************************************************
  * @file    bsp_vofa.c
  * @brief   VOFA+ 协议实现
  * @note    STM32 是小端，float 在内存里本来就是小端字节序，
  *          所以可以直接把 float 数组的地址强转成 uint8_t* 发出去。
  ******************************************************************************
  */
#include "bsp_vofa.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/* JustFloat 协议的帧尾，固定 4 个字节，不能改。
   它按小端解释是 0x7F800000，也就是正无穷，
   正常数据不会出现这个值，所以能安全地当分隔符用。 */
static const uint8_t vofa_tail[4] = {0x00, 0x00, 0x80, 0x7F};

/**
  * @brief  按 JustFloat 协议发送一组 float
  * @param  data 数据数组首地址
  * @param  num  元素个数
  * @note   帧格式： float1 float2 ... floatN | 00 00 80 7F
  */
void Vofa_JustFloat(float *data, uint8_t num)
{
    /* 先发数据本体：num 个 float，每个 4 字节 */
    HAL_UART_Transmit(&huart1, (uint8_t *)data, (uint16_t)(num * 4), 100);

    /* 再发帧尾，VOFA+ 靠它判断"这一帧结束了" */
    HAL_UART_Transmit(&huart1, (uint8_t *)vofa_tail, 4, 100);
}

/**
  * @brief  按 FireWater 协议发送一行文本
  * @param  fmt 格式串
  * @note   本质就是 printf 到串口。newlib-nano 不支持 %f，
  *         要打印小数请先乘 10 或 100 变成整数再发。
  */
void Vofa_FireWater(const char *fmt, ...)
{
    char buf[64];
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    HAL_UART_Transmit(&huart1, (uint8_t *)buf, (uint16_t)strlen(buf), 100);
}
