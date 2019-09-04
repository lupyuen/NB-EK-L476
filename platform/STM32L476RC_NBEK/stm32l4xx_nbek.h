/**
  ******************************************************************************
  * @file    stm32l4xx_nucleo.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for:
  *          - LEDs and push-button available on STM32L4XX-Nucleo Kit
  *            from STMicroelectronics
  *          - LCD, joystick and microSD available on Adafruit 1.8" TFT LCD
  *            shield (reference ID 802), external SMPS
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L4XX_NBEK_H
#define __STM32L4XX_NBEK_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32L4XX_NBEK
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

//*****************************************************************************
// 中断函数指针类型
typedef void (*IrqFuncPtr)(void);

typedef enum
{
  LED0,
  LEDn
} Led_TypeDef;

typedef enum
{
	BTN_NONE,
	BTN_UP,
	BTN_RIGHT,
	BTN_DOWN,
	BTN_LEFT
} Button_TypeDef;


/**
* @brief  Define for STM32L4XX_NBEK board
  */
#if !defined (USE_STM32L4XX_NBEK)
#define USE_STM32L4XX_NBEK
#endif

/** @defgroup STM32L4XX_NBEK_LED LED 
  * @{
  */
//#define LEDn                               1

#define LED0_PIN                           GPIO_PIN_15
#define LED0_GPIO_PORT                     GPIOA


/**
  * @}
  */

/** @defgroup STM32L4XX_NBEK_BUTTON BUTTON
  * @{
  */
#define BUTTONn                            4

/**
  * @brief Key push-button
  */
#define BTN_UP_PIN                              GPIO_PIN_3
#define BTN_UP_GPIO_PORT                        GPIOC
#define BTN_UP_EXTI_LINE                        GPIO_PIN_3
#define BTN_UP_EXTI_IRQn                        EXTI3_IRQn

#define BTN_RIGHT_PIN                           GPIO_PIN_2
#define BTN_RIGHT_GPIO_PORT                     GPIOC
#define BTN_RIGHT_EXTI_LINE                     GPIO_PIN_2
#define BTN_RIGHT_EXTI_IRQn                     EXTI2_IRQn

#define BTN_DOWN_PIN                            GPIO_PIN_1
#define BTN_DOWN_GPIO_PORT                      GPIOC
#define BTN_DOWN_EXTI_LINE                      GPIO_PIN_1
#define BTN_DOWN_EXTI_IRQn                      EXTI1_IRQn

#define BTN_LEFT_PIN                            GPIO_PIN_0
#define BTN_LEFT_GPIO_PORT                      GPIOC
#define BTN_LEFT_EXTI_LINE                      GPIO_PIN_0
#define BTN_LEFT_EXTI_IRQn                      EXTI0_IRQn

/**
  * @}
  */


/** @defgroup STM32L4XX_NUCLEO_BUS BUS
  * @{
  */
/*###################### SPI1 ###################################*/

#define LCD_SPIx                                  SPI1
#define LCD_SPIx_CLK_ENABLE()                    __HAL_RCC_SPI1_CLK_ENABLE()

#define LCD_SPIx_SCK_AF                          GPIO_AF5_SPI1
#define LCD_SPIx_SCK_GPIO_PORT                   GPIOA
#define LCD_SPIx_SCK_PIN                         GPIO_PIN_5

#define LCD_SPIx_MISO_MOSI_AF                    GPIO_AF5_SPI1
#define LCD_SPIx_MISO_MOSI_GPIO_PORT             GPIOA
#define LCD_SPIx_MISO_PIN                        GPIO_PIN_7

/**
  * @brief  LCD Backlight Control Interface pins
  */
#define LCD_BL_PIN                                 GPIO_PIN_0
#define LCD_BL_GPIO_PORT                           GPIOB


/**
  * @brief  LCD Control Interface pins
  */
#define LCD_CS_PIN                                 GPIO_PIN_4
#define LCD_CS_GPIO_PORT                           GPIOA

/**
  * @brief  LCD Data/Command Interface pins
  */
#define LCD_CD_PIN                                 GPIO_PIN_6
#define LCD_CD_GPIO_PORT                           GPIOA

/*##################### USART ##################################*/

/**
  * @brief  USART1 Interface pins
  *         used to print log infomation
  */

#define LOGx                            USART1
#define LOGx_CLK_ENABLE()             __HAL_RCC_USART1_CLK_ENABLE()
#define LOGx_CLK_DISABLE()            __HAL_RCC_USART1_CLK_DISABLE()

#define LOGx_TXRX_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()  // __HAL_RCC_GPIOA_CLK_ENABLE()


#define LOGx_FORCE_RESET()            __HAL_RCC_USART1_FORCE_RESET()
#define LOGx_RELEASE_RESET()          __HAL_RCC_USART1_RELEASE_RESET()

/* Definition for USARTx Pins */
#define LOGx_TX_PIN                   GPIO_PIN_9     // GPIO_PIN_9
#define LOGx_TX_GPIO_PORT             GPIOA          // GPIOA

#define LOGx_RX_PIN                   GPIO_PIN_10    // GPIO_PIN_10
#define LOGx_RX_GPIO_PORT             GPIOA          // GPIOA
#define LOGx_GPIO_PORT                GPIOA 
#define LOGx_AF                       GPIO_AF7_USART1


/**
  * @brief  LPUART1 Interface pins
  *         used to communicate with nb device
  */

#define NBUart                          LPUART1
#define NBUart_CLK_ENABLE()             __HAL_RCC_LPUART1_CLK_ENABLE()
#define NBUart_CLK_DISABLE()            __HAL_RCC_LPUART1_CLK_DISABLE()

#define NBUart_TXRX_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()  // __HAL_RCC_GPIOA_CLK_ENABLE()

#define NBUart_FORCE_RESET()            __HAL_RCC_LPUART1_FORCE_RESET()
#define NBUart_RELEASE_RESET()          __HAL_RCC_LPUART1_RELEASE_RESET()

/* Definition for LPUSARTx Pins */
#define NBUart_TX_PIN                   GPIO_PIN_11     // GPIO_PIN_11
#define NBUart_TX_GPIO_PORT             GPIOB           // GPIOB

#define NBUart_RX_PIN                   GPIO_PIN_10     // GPIO_PIN_10
#define NBUart_RX_GPIO_PORT             GPIOB           // GPIOB
#define NBUart_GPIO_PORT                GPIOB 
#define NBUart_AF                       GPIO_AF8_LPUART1

/* NB power enPin*/
#define NB_PWR_PIN                      GPIO_PIN_0
#define NB_PWR_PORT                     GPIOA


/**
  * @brief  USART2 Interface pins
  *         used to communicate with GPS
  */
#define GPSUart                         USART2
#define GPSUart_CLK_ENABLE()            __HAL_RCC_USART2_CLK_ENABLE()
#define GPSUart_CLK_DISABLE()           __HAL_RCC_USART2_CLK_DISABLE()

/* Definition for USART2 Pins */
#define GPSUart_TX_PIN                  GPIO_PIN_2     // GPIO_PIN_11
#define GPSUart_TX_GPIO_PORT            GPIOA           // GPIOB

#define GPSUart_RX_PIN                  GPIO_PIN_3     // GPIO_PIN_10
#define GPSUart_RX_GPIO_PORT            GPIOA           // GPIOB
#define GPSUart_GPIO_PORT               GPIOA 
#define GPSUart_AF                      GPIO_AF7_USART2

/* GPS power enPin*/
#define GPS_PWR_PIN                      GPIO_PIN_1
#define GPS_PWR_PORT                     GPIOA

/*##################### ADC1 ###################################*/
/**
  * @brief  ADC Interface pins
  *         used to detect bright of envirment
  */
#define LUX_ADCx                                 ADC1
#define LUX_ADCx_CLK_ENABLE()                    __HAL_RCC_ADC_CLK_ENABLE()
#define LUX_ADCx_CLK_DISABLE()                   __HAL_RCC_ADC_CLK_DISABLE()

#define LUX_ADCx_GPIO_PORT                       GPIOB
#define LUX_ADCx_GPIO_PIN                        GPIO_PIN_1


/*##################### I2C ###################################*/
#define SHT2x_I2C                                   I2C2
#define SHT2x_I2C_CLK_ENABLE()                      __HAL_RCC_I2C2_CLK_ENABLE()
#define SHT2x_I2C_CLK_DISABLE()                     __HAL_RCC_I2C2_CLK_DISABLE()

#define SHT2x_I2C_FORCE_RESET()                     __HAL_RCC_I2C2_FORCE_RESET()
#define SHT2x_I2C_RELEASE_RESET()                   __HAL_RCC_I2C2_RELEASE_RESET()

/* Definition for i2c2 Pins */
#define SHT2x_I2C_SCL_PIN                           GPIO_PIN_13
#define SHT2x_I2C_SCL_PORT                          GPIOB

#define SHT2x_I2C_SDA_PIN                           GPIO_PIN_14
#define SHT2x_I2C_SDA_PORT                          GPIOB

#define SHT2x_I2C_AF                                GPIO_AF4_I2C2
#define SHT2x_I2C_GPIO_MODE                         GPIO_MODE_AF_OD
#define SHT2x_I2C_PORT                              GPIOB

/*##################### TIME ###################################*/
#define LIGHT_TIM                                   LPTIM1
#define LIGHT_TIM_CLK_ENABLE()                       __HAL_RCC_LPTIM1_CLK_ENABLE()
#define LIGHT_TIM_CLK_DISABLE()                      __HAL_RCC_LPTIM1_CLK_DISABLE()

#define LIGHT_TIM_GPIO_PORT                          GPIOB
#define LIGHT_TIM_GPIO_PIN                           GPIO_PIN_2
#define LIGHT_TIM_AF                                 GPIO_AF1_LPTIM1



/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup STM32L4XX_NUCLEO_Exported_Functions
  * @{
  */
uint32_t         BSP_GetVersion(void);


/**
  * @}
  */



#ifdef __cplusplus
}
#endif

#endif /* __STM32L4XX_NBEK_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

