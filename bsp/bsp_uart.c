/**
 ******************************************************************************
 * @file    bsp_uart.c
 * @brief   UART 通信的板级封装实现
 * @note    这里的函数名、顺序、注释都和《3.2 UART》教程保持一致，
 *          可以对着视频一行一行敲。工程里额外加了错误回调，
 *          是为了防止溢出后串口"假死"，教程里没有这一段，删掉也能跑。
 ******************************************************************************
 */
#include "bsp_uart.h"
#include <string.h>

uint8_t Receive_Data[1];
volatile uint8_t cur_mode = '1';
/**
 * @brief  通用发送
 * @param  huart     串口句柄，例如 &huart1
 * @param  send_buf  要发数据的首地址
 * @param  send_size 字节数
 * @param  mode      阻塞 / 中断 / DMA
 */
void USARTsend(UART_HandleTypeDef *huart, uint8_t *send_buf,
               uint16_t send_size, USART_TRANSFER_MODE mode)
{
    switch (mode)
    {
    case USART_BLOCK: /* 阻塞发送：CPU 一直等 */
        HAL_UART_Transmit(huart, send_buf, send_size, HAL_MAX_DELAY);
        break;

    case USART_IT: /* 中断发送：发完进回调 */
        HAL_UART_Transmit_IT(huart, send_buf, send_size);
        break;

    case USART_DMA: /* DMA 发送：不占 CPU */
        HAL_UART_Transmit_DMA(huart, send_buf, send_size);
        break;

    default:
        break;
    }
}

/**
 * @brief  启动第一次中断接收
 * @note   必须在 MX_USART1_UART_Init() 之后调用。
 *         以后每一帧都由回调里的 HAL_UART_Receive_IT 接力，
 *         所以只需要在开机时调用这一次。
 */
void BSP_UART_Init(void) // 接收数据
{
    HAL_UART_Receive_IT(&huart1, Receive_Data, 1);
}

/**
 * @brief  发一个字符串（教程里没有，方便打印调试信息）
 */
void BSP_UART_SendString(UART_HandleTypeDef *huart, const char *str)
{
    HAL_UART_Transmit(huart, (uint8_t *)str, (uint16_t)strlen(str), HAL_MAX_DELAY);
}

/**
 * @brief  发一个字节（教程里没有）
 */
void BSP_UART_SendByte(UART_HandleTypeDef *huart, uint8_t byte)
{
    HAL_UART_Transmit(huart, &byte, 1, HAL_MAX_DELAY);
}

/* ========================================================================== */
/*                          以下是 HAL 的回调函数                              */
/* ========================================================================== */

/* 帧头帧尾方式下，要发出去的 2 字节数据 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) // 接收完数据之后转到这里
{
    uint8_t c = Receive_Data[0];
    if (c == '1' || c == '2' || c == '3')
        cur_mode = c;
    HAL_UART_Receive_IT(&huart1, Receive_Data, 1);
}

/**
 * @brief  串口出错（最常见的是溢出 Overrun，ORE 标志置位）
 * @note   这一段教程里没有。溢出后 HAL 会停止接收，不重新武装的话串口就
 *         "发一次有反应、之后怎么发都没反应"。不需要可以直接删掉。
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        __HAL_UART_CLEAR_OREFLAG(huart);
        HAL_UART_Receive_IT(&huart1, Receive_Data, 1);
    }
}
