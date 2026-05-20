/**
  ******************************************************************************
  * @author  调试串口驱动
  * @version V1.0
  * @date    2026-05-19
  * @brief   UART3调试配置H文件
  ******************************************************************************
  */

#ifndef __DRV_UART3_H__
#define __DRV_UART3_H__

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include <stdio.h>
#include <stdarg.h>

#define UART3_DEBUG_ENABLE			0

//UART3硬件定义
#define UART3_PORT					USART3
#define UART3_PORT_CLK				RCC_APB1Periph_USART3

//UART3引脚定义 (PB10=TX, PB11=RX)
#define UART3_TX_GPIO_PORT			GPIOB
#define UART3_TX_GPIO_CLK			RCC_APB2Periph_GPIOB
#define UART3_TX_GPIO_PIN			GPIO_Pin_10

#define UART3_RX_GPIO_PORT			GPIOB
#define UART3_RX_GPIO_CLK			RCC_APB2Periph_GPIOB
#define UART3_RX_GPIO_PIN			GPIO_Pin_11

#if UART3_DEBUG_ENABLE
void drv_uart3_init( uint32_t BaudRate );
void drv_uart3_tx_bytes( uint8_t* TxBuffer, uint8_t Length );
void drv_uart3_printf( const char* fmt, ... );
#else
#define drv_uart3_init( ... )		((void)0)
#define drv_uart3_tx_bytes( ... )	((void)0)
#define drv_uart3_printf( ... )		((void)0)
#endif

#endif
