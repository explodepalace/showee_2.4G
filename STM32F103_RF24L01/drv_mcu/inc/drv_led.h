/**
  ******************************************************************************
  * @author  泽耀科技 ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   LED配置H文件
  ******************************************************************************
  * @attention
  *
  * 官网	:	http://www.ashining.com
  * 淘宝	:	https://shop105912646.taobao.com
  * 阿里巴巴:	https://cdzeyao.1688.com
  ******************************************************************************
  */


#ifndef	__DRV_LED_H__
#define __DRV_LED_H__


#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"



//LED硬件定义 (单个LED PC13)
#define LED_GPIO_PORT				GPIOC
#define LED_GPIO_CLK				RCC_APB2Periph_GPIOC
#define LED_GPIO_PIN				GPIO_Pin_13

//LED硬件定义 (单个LED PC13)
#define LED_GPIO_PORT				GPIOC
#define LED_GPIO_CLK				RCC_APB2Periph_GPIOC
#define LED_GPIO_PIN				GPIO_Pin_13

/** LED闪烁模式定义 */
typedef enum LedFlashMode
{
	LED_FLASH_FAST = 0,		//快速闪烁 (100ms周期) - TX模式
	LED_FLASH_SLOW			//慢速闪烁 (250ms周期) - RX模式
}LedFlashModeType;


void drv_led_init( void );
void drv_led_on( void );
void drv_led_off( void );
void drv_led_flash( LedFlashModeType Mode );

#define led_on( )				drv_led_on( )
#define led_off( )				drv_led_off( )
#define led_flash_fast( )		drv_led_flash( LED_FLASH_FAST )
#define led_flash_slow( )		drv_led_flash( LED_FLASH_SLOW )


#endif

