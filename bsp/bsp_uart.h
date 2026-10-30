/**
 ******************************************************************************
 * @file    bsp_uart.h
 * @brief   UART 通信的板级封装（BSP 层）
 * @note    函数命名与《3.2 UART》教程保持一致，方便对照视频逐行敲。
 ******************************************************************************
 */
#ifndef __BSP_UART_H
#define __BSP_UART_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "usart.h"

  /* 发送方式：对应 USARTsend 的第四个参数 */
  typedef enum
  {
    USART_BLOCK = 0, /* 阻塞发送 */
    USART_IT,        /* 中断发送 */
    USART_DMA        /* DMA 发送 */
  } USART_TRANSFER_MODE;

  /* 接收缓冲区：一帧 4 字节，帧格式  'A' | 数据1 | 数据2 | 'B'  */
  extern uint8_t Receive_Data[1];
  extern volatile uint8_t cur_mode;
  /* 校验通过后要回发的有效数据，2 字节 */

  /* 通用发送：一个函数覆盖阻塞 / 中断 / DMA 三种模式 */
  void USARTsend(UART_HandleTypeDef *huart, uint8_t *send_buf,
                 uint16_t send_size, USART_TRANSFER_MODE mode);

  /* 启动第一次中断接收，必须在 MX_USART1_UART_Init() 之后调用 */
  void BSP_UART_Init(void);

  /* 两个便捷函数，教程里没有，用着方便 */
  void BSP_UART_SendString(UART_HandleTypeDef *huart, const char *str);
  void BSP_UART_SendByte(UART_HandleTypeDef *huart, uint8_t byte);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_UART_H */
