/**
  ******************************************************************************
  * @author  泽耀科技 ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   UART配置C文件
  ******************************************************************************
  * @attention
  *
  * 官网	:	http://www.ashining.com
  * 淘宝	:	https://shop105912646.taobao.com
  * 阿里巴巴:	https://cdzeyao.1688.com
  ******************************************************************************
  */


#include "drv_uart.h"
#include "misc.h"
#include "perf_time.h"


#define UART_RX_BUFFER_SIZE		100

static volatile uint8_t s_UartRxBuffer[ UART_RX_BUFFER_SIZE ] = { 0 };
static volatile uint8_t s_UartRxLength = 0;
static volatile uint8_t s_UartRxFrameReady = 0;
static volatile uint32_t s_UartRxDoneTick = 0;


/**
  * @brief :串口初始化
  * @param :
  *			@UartBaudRate:串口波特率
  * @note  :无
  * @retval:无
  */
void drv_uart_init( uint32_t UartBaudRate )
{
	GPIO_InitTypeDef	UartGpioInitStructer;
	USART_InitTypeDef	UartinitStructer;
	NVIC_InitTypeDef	UartNvicInitStructer;
	
	//在配置过程中，为防止TX RX不再同一个端口上，增强可移植性，固分开配置
	//初始化串口TX RX 引脚 
	RCC_APB2PeriphClockCmd( UART_TX_GPIO_CLK | UART_RX_GPIO_CLK, ENABLE );	//打开TX RX 端口时钟
	
	UartGpioInitStructer.GPIO_Mode = GPIO_Mode_AF_PP;
	UartGpioInitStructer.GPIO_Speed = GPIO_Speed_2MHz;
	//TX
	UartGpioInitStructer.GPIO_Pin = UART_TX_GPIO_PIN;
	GPIO_Init( UART_TX_GPIO_PORT, &UartGpioInitStructer );		//初始化TX引脚  配置为复用功能
	//RX
	UartGpioInitStructer.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_AF_PP;//GPIO_Mode_AF_OD;//GPIO_Mode_AIN;//GPIO_Mode_IN_FLOATING;
	UartGpioInitStructer.GPIO_Pin = UART_RX_GPIO_PIN;
	GPIO_Init( UART_RX_GPIO_PORT, &UartGpioInitStructer );		//初始化RX引脚  配置为输入
	
	//配置USART外设
	USART_DeInit( UART_PORT );		//外设复位
	
	if( USART1 == UART_PORT )		//使能外设时钟
	{
		RCC_APB2PeriphClockCmd( UART_PORT_CLK, ENABLE );			
	}																	//不同的USART外设可能在不同的APB时钟上														
	else																//STM32F103单片机只有USART1在APB2上，如平台有差异做相应改变即可
	{
		RCC_APB1PeriphClockCmd( UART_PORT_CLK, ENABLE );
	}
	
	UartinitStructer.USART_BaudRate = UartBaudRate;						//设置波特率
	UartinitStructer.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//不使用流控制
	UartinitStructer.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		//发送和接收	
	UartinitStructer.USART_Parity = USART_Parity_No;					//不带校验
	UartinitStructer.USART_StopBits = USART_StopBits_1;					//一个停止位
	UartinitStructer.USART_WordLength = USART_WordLength_8b;			//8个数据位
	
	USART_Cmd( UART_PORT, DISABLE );									//失能外设
	USART_Init( UART_PORT, &UartinitStructer );							//初始化外设
	USART_ITConfig( UART_PORT, USART_IT_RXNE, ENABLE );

	USART_ITConfig( UART_PORT, USART_IT_RXNE, ENABLE );

	UartNvicInitStructer.NVIC_IRQChannel = USART1_IRQn;
	UartNvicInitStructer.NVIC_IRQChannelPreemptionPriority = 1;
	UartNvicInitStructer.NVIC_IRQChannelSubPriority = 1;
	UartNvicInitStructer.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &UartNvicInitStructer );

	USART_Cmd( UART_PORT, ENABLE );										//使能外设	
}

/**
  * @brief :串口发送数据
  * @param :
  *			@TxBuffer:发送数据首地址
  *			@Length:数据长度
  * @note  :无
  * @retval:无
  */
void drv_uart_tx_bytes( uint8_t* TxBuffer, uint8_t Length )
{
	while( Length-- )
	{
		while( RESET == USART_GetFlagStatus( UART_PORT, USART_FLAG_TXE ));
		UART_PORT->DR = *TxBuffer;
		TxBuffer++;
	}
}

/**
  * @brief :串口接收数据
  * @param :
  *			@RxBuffer:发送数据首地址
  * @note  :无
  * @retval:接收到的字节个数
  */
uint8_t drv_uart_rx_bytes( uint8_t* RxBuffer )
{
	uint8_t l_Index = 0;
	uint8_t l_RxLength = 0;

	if( 0 == s_UartRxFrameReady )
	{
		return 0;
	}

	USART_ITConfig( UART_PORT, USART_IT_RXNE, DISABLE );
	l_RxLength = s_UartRxLength;
	for( l_Index = 0; l_Index < l_RxLength; l_Index++ )
	{
		RxBuffer[ l_Index ] = s_UartRxBuffer[ l_Index ];
	}
	s_UartRxLength = 0;
	s_UartRxFrameReady = 0;
	USART_ITConfig( UART_PORT, USART_IT_RXNE, ENABLE );

	return l_RxLength;
}

uint32_t drv_uart_rx_done_us( void )
{
	return perf_time_tick_to_us( s_UartRxDoneTick );
}

void USART1_IRQHandler( void )
{
	uint8_t l_Char;

	if( RESET != USART_GetITStatus( USART1, USART_IT_RXNE ) )
	{
		l_Char = (uint8_t)USART1->DR;

		if( 0 != s_UartRxFrameReady )
		{
			return;
		}

		if( '\r' == l_Char )
		{
			if( 0 != s_UartRxLength )
			{
				s_UartRxDoneTick = perf_time_now_tick( );
				s_UartRxFrameReady = 1;
			}
			return;
		}

		if( '\n' == l_Char )
		{
			return;
		}

		if( s_UartRxLength < UART_RX_BUFFER_SIZE )
		{
			s_UartRxBuffer[ s_UartRxLength++ ] = l_Char;
			if( UART_RX_BUFFER_SIZE == s_UartRxLength )
			{
				s_UartRxFrameReady = 1;
			}
		}
		else
		{
			s_UartRxFrameReady = 1;
		}
	}
}

