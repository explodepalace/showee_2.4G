/**
  ******************************************************************************
  * @author  调试串口驱动
  * @version V1.0
  * @date    2026-05-19
  * @brief   UART3调试配置C文件
  ******************************************************************************
  */

#include "drv_uart3.h"

/**
  * @brief :UART3初始化
  * @param :@BaudRate:串口波特率
  * @note  :无
  * @retval:无
  */
void drv_uart3_init( uint32_t BaudRate )
{
	GPIO_InitTypeDef	GpioInitStructer;
	USART_InitTypeDef	UartinitStructer;

	//初始化UART3 TX RX 引脚
	RCC_APB2PeriphClockCmd( UART3_TX_GPIO_CLK | UART3_RX_GPIO_CLK, ENABLE );

	GpioInitStructer.GPIO_Mode = GPIO_Mode_AF_PP;
	GpioInitStructer.GPIO_Speed = GPIO_Speed_2MHz;
	//TX
	GpioInitStructer.GPIO_Pin = UART3_TX_GPIO_PIN;
	GPIO_Init( UART3_TX_GPIO_PORT, &GpioInitStructer );
	//RX
	GpioInitStructer.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GpioInitStructer.GPIO_Pin = UART3_RX_GPIO_PIN;
	GPIO_Init( UART3_RX_GPIO_PORT, &GpioInitStructer );

	//配置USART3外设
	USART_DeInit( UART3_PORT );
	RCC_APB1PeriphClockCmd( UART3_PORT_CLK, ENABLE );

	UartinitStructer.USART_BaudRate = BaudRate;
	UartinitStructer.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	UartinitStructer.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	UartinitStructer.USART_Parity = USART_Parity_No;
	UartinitStructer.USART_StopBits = USART_StopBits_1;
	UartinitStructer.USART_WordLength = USART_WordLength_8b;

	USART_Cmd( UART3_PORT, DISABLE );
	USART_Init( UART3_PORT, &UartinitStructer );
	USART_Cmd( UART3_PORT, ENABLE );
}

/**
  * @brief :UART3发送数据
  * @param :@TxBuffer:发送数据首地址, @Length:数据长度
  * @note  :无
  * @retval:无
  */
void drv_uart3_tx_bytes( uint8_t* TxBuffer, uint8_t Length )
{
	while( Length-- )
	{
		while( RESET == USART_GetFlagStatus( UART3_PORT, USART_FLAG_TXE ));
		UART3_PORT->DR = *TxBuffer;
		TxBuffer++;
	}
}

/**
  * @brief :UART3 printf风格输出
  * @param :@fmt:格式字符串, ...:可变参数
  * @note  :无
  * @retval:无
  */
void drv_uart3_printf( const char* fmt, ... )
{
	char buffer[256];
	va_list args;
	int len;

	va_start(args, fmt);
	len = vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);

	if(len > 0)
	{
		drv_uart3_tx_bytes((uint8_t*)buffer, len);
	}
}
