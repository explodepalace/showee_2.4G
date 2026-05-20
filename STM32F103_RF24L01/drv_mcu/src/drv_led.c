/**
  ******************************************************************************
  * @author  泽耀科技 ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   LED配置C文件
  ******************************************************************************
  * @attention
  *
  * 官网	:	http://www.ashining.com
  * 淘宝	:	https://shop105912646.taobao.com
  * 阿里巴巴:	https://cdzeyao.1688.com
  ******************************************************************************
  */



#include "drv_led.h"



/**
  * @brief :LED初始化
  * @param :无
  * @note  :无
  * @retval:无
  */ 
void drv_led_init( void )
{
	GPIO_InitTypeDef	GpioInitStructer;

	RCC_APB2PeriphClockCmd( LED_GPIO_CLK, ENABLE );

	GpioInitStructer.GPIO_Mode = GPIO_Mode_Out_PP;
	GpioInitStructer.GPIO_Speed = GPIO_Speed_2MHz;
	GpioInitStructer.GPIO_Pin = LED_GPIO_PIN;
	GPIO_Init( LED_GPIO_PORT, &GpioInitStructer );
	GPIO_SetBits( LED_GPIO_PORT, LED_GPIO_PIN );

}

/**
  * @brief :LED亮
  * @param :
  *			@LedPort:LED选择，红色或蓝色
  * @note  :无
  * @retval:无
  */
void drv_led_on( void )
{
	GPIO_ResetBits( LED_GPIO_PORT, LED_GPIO_PIN );
}

/**
  * @brief :LED灭
  * @param :无
  * @note  :无
  * @retval:无
  */
void drv_led_off( void )
{
	GPIO_SetBits( LED_GPIO_PORT, LED_GPIO_PIN );
}

/**
  * @brief :LED闪烁
  * @param :
  *			@Mode:闪烁模式（快速或慢速）
  * @note  :无
  * @retval:无
  */
void drv_led_flash( LedFlashModeType Mode )
{
	if( LED_FLASH_FAST == Mode )
	{
		drv_delay_100Ms( 1 );
		LED_GPIO_PORT->ODR ^= (uint32_t)LED_GPIO_PIN;
		drv_delay_100Ms( 1 );
		LED_GPIO_PORT->ODR ^= (uint32_t)LED_GPIO_PIN;
	}
	else
	{
		drv_delay_250Ms( 1 );
		LED_GPIO_PORT->ODR ^= (uint32_t)LED_GPIO_PIN;
		drv_delay_250Ms( 1 );
		LED_GPIO_PORT->ODR ^= (uint32_t)LED_GPIO_PIN;
	}
}
