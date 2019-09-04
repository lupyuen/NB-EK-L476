/**
  ******************************************************************************
  * @file    stm32l4xx_nucleo.c
  * @author  MCD Application Team
  * @brief   This file provides set of firmware functions to manage:
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

/* Includes ------------------------------------------------------------------*/

#include "los_typedef.h"
#include "los_hwi.h"
#include "stm32l4xx_nbek.h"
#include "stm32l4xx_it.h"
/** @addtogroup BSP
  * @{
  */

/** @defgroup STM32L4XX_NUCLEO NUCLEO 64
  * @brief This file provides set of firmware functions to manage Leds and push-button
  *        available on STM32L4XX-Nucleo Kit from STMicroelectronics.
  *        It provides also LCD, joystick and uSD functions to communicate with
  *        Adafruit 1.8" TFT LCD shield (reference ID 802)
  * @{
  */


/** @defgroup STM32L4XX_NUCLEO_Private_Constants Private Constants
  * @{
  */

/**
  * @brief STM32L4XX NUCLEO BSP Driver version number
  */
#define __STM32L4XX_NUCLEO_BSP_VERSION_MAIN   (0x02) /*!< [31:24] main version */
#define __STM32L4XX_NUCLEO_BSP_VERSION_SUB1   (0x01) /*!< [23:16] sub1 version */
#define __STM32L4XX_NUCLEO_BSP_VERSION_SUB2   (0x00) /*!< [15:8]  sub2 version */
#define __STM32L4XX_NUCLEO_BSP_VERSION_RC     (0x00) /*!< [7:0]  release candidate */
#define __STM32L4XX_NUCLEO_BSP_VERSION       ((__STM32L4XX_NUCLEO_BSP_VERSION_MAIN << 24)\
                                             |(__STM32L4XX_NUCLEO_BSP_VERSION_SUB1 << 16)\
                                             |(__STM32L4XX_NUCLEO_BSP_VERSION_SUB2 << 8 )\
                                             |(__STM32L4XX_NUCLEO_BSP_VERSION_RC))
/** @defgroup STM32L4XX_NBEK interrupte function ptr
  * @{
  */


/**
  * @brief LINK SD Card
  */
#define SD_DUMMY_BYTE            0xFF
#define SD_NO_RESPONSE_EXPECTED  0x80


#ifdef USE_STM32L4XX_NUCLEO_64_SMPS

/**
  * @brief SMPS
  */


#ifdef USE_ADP5301ACBZ          /* ADP5301ACBZ */

/* ######################################################################## */
/* #define PORT_SMPS               GPIOA                                    */
/* #define PIN_SMPS_ENABLE         GPIO_PIN_4                               */
/* #define PIN_SMPS_POWERGOOD      GPIO_PIN_6                               */
/* #define PIN_SMPS_SWITCH_ENABLE  GPIO_PIN_7                               */

/* IN CASE OF SMPS VOLTAGE RANGE SELECTION                                  */
/* #define PIN_SMPS_V1             GPIO_PIN_5                               */
/* ######################################################################## */

#endif                          /* ADP5301ACBZ */

/**
  * @}
  */

#endif /* USE_STM32L4XX_NUCLEO_64_SMPS */


#ifdef HAL_SPI_MODULE_ENABLED

#endif /* HAL_SPI_MODULE_ENABLED */

#ifdef HAL_ADC_MODULE_ENABLED
static ADC_HandleTypeDef hnucleo_Adc;
/* ADC channel configuration structure declaration */
static ADC_ChannelConfTypeDef hnucleo_AdcChannelConfig;
#endif /* HAL_ADC_MODULE_ENABLED */

/**
  * @}
  */

/* Private Function Prototypes */
#ifdef HAL_SPI_MODULE_ENABLED

#endif /* HAL_SPI_MODULE_ENABLED */

#ifdef HAL_ADC_MODULE_ENABLED

#endif /* HAL_ADC_MODULE_ENABLED */

#ifdef HAL_SPI_MODULE_ENABLED
/* SD IO functions */
void                      SD_IO_Init(void);
void                      SD_IO_CSState(uint8_t state);
void                      SD_IO_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength);
uint8_t                   SD_IO_WriteByte(uint8_t Data);

/* LCD IO functions */
void                      LCD_IO_Init(void);
void                      LCD_IO_WriteMultipleData(uint8_t *pData, uint32_t Size);
void                      LCD_IO_WriteReg(uint8_t LCDReg);
void                      LCD_Delay(uint32_t delay);
#endif /* HAL_SPI_MODULE_ENABLED */

/** @defgroup STM32L4XX_NUCLEO_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief  This method returns the STM32L4XX NUCLEO BSP Driver revision
  * @retval version : 0xXYZR (8bits for each decimal, R for RC)
  */
uint32_t BSP_GetVersion(void)
{
  return __STM32L4XX_NUCLEO_BSP_VERSION;
}



/** @defgroup STM32L4XX_NUCLEO_BUTTON_Functions BUTTON Functions
  * @{
  */

/**
  * @brief  Configure Button GPIO and EXTI Line.
  * @param  Button: Specifies the Button to be configured.
  *   This parameter should be: BUTTON_USER
  * @param  ButtonMode: Specifies Button mode.
  *   This parameter can be one of following parameters:
  *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                            generation capability
  * @retval None
  */
//void BSP_PB_Init(void)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};

//  /* Enable the BUTTON Clock */
//  BUTTONx_GPIO_CLK_ENABLE(BTN_UP);

//	/* Configure Button pin as input with External interrupt */
//	GPIO_InitStruct.Pin    = BTN_UP_PIN|BTN_RIGHT_PIN|BTN_DOWN_PIN|BTN_LEFT_PIN;
//	GPIO_InitStruct.Mode   = GPIO_MODE_IT_FALLING;
//	GPIO_InitStruct.Pull   = GPIO_PULLUP;
//	GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_HIGH;

//	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

//	/* Enable and set Button EXTI Interrupt to the lowest priority */
//	for(uint8_t i = 0 ; i < BUTTONn ; i++)
//	{
//		UINTPTR uvIntSave;
//		uvIntSave = LOS_IntLock();
//		
//		LOS_HwiCreate(BUTTON_IRQn[i], 0xF,0,BUTTON_IRQFunc[i],0);//´´½¨ÖÐ¶Ï
//		
//		LOS_IntRestore(uvIntSave);
//	}
//}

/**
  * @brief  DeInitialize Push Button.
  * @param  Button: Button to be configured
  *   This parameter should be: BUTTON_USER
  * @note BSP_PB_DeInit() does not disable the GPIO clock
  * @retval None
  */
//void BSP_PB_DeInit(void)
//{
//  GPIO_InitTypeDef GPIO_InitStruct;

//  GPIO_InitStruct.Pin = BTN_UP_PIN|BTN_RIGHT_PIN|BTN_DOWN_PIN|BTN_LEFT_PIN;
//	for(uint8_t i = 0 ; i < BUTTONn ; i++)
//	{
//    LOS_HwiDelete(BUTTON_IRQn[i]);
//	}
//  HAL_GPIO_DeInit(GPIOC, GPIO_InitStruct.Pin);
//}

/**
  * @brief  Return the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  * @retval Button state.
  */
//uint32_t BSP_PB_GetState(Button_TypeDef Button)
//{
//  if(Button == BTN_NONE)
//  {
//    return 0xFF;
//  }
//  return HAL_GPIO_ReadPin(BUTTON_PORT[Button - 1], BUTTON_PIN[Button - 1]);
//}

#ifdef USE_STM32L4XX_NUCLEO_64_SMPS
#ifdef USE_ADP5301ACBZ          /* ADP5301ACBZ */

/** @defgroup STM32L4XX_NUCLEO_SMPS_Functions SMPS Functions
  * @{
  */

/******************************************************************************
                            SMPS OPERATIONS
*******************************************************************************/

/**
  * @brief  DeInitialize the external SMPS component
  * @note   Low power consumption GPIO settings
  * @retval SMPS status
  */
uint32_t BSP_SMPS_DeInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  PWR_AND_CLK_SMPS();

  /* --------------------------------------------------------------------------------------  */
  /* Added for Deinit if No PIN_SMPS_ENABLE & PIN_SMPS_SWITCH_ENABLE are not disabled before */

  /* Disable SMPS SWITCH */
  HAL_GPIO_WritePin(PORT_SMPS, PIN_SMPS_SWITCH_ENABLE, GPIO_PIN_RESET);

  HAL_Delay(1);

  /* Disable SMPS */
  HAL_GPIO_WritePin(PORT_SMPS, PIN_SMPS_ENABLE, GPIO_PIN_RESET);

  /* --------------------------------------------------------------------------------------  */
  /* Set all GPIO in output push/pull pulldown state to reduce power consumption  */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Pull =  GPIO_PULLDOWN;

  /* Consider all SMPS pins but V1, not used on ADP5301ACBZ */
  GPIO_InitStruct.Pin = PIN_SMPS_ENABLE | PIN_SMPS_SWITCH_ENABLE | PIN_SMPS_POWERGOOD;
  HAL_GPIO_Init(PORT_SMPS, &GPIO_InitStruct);

  return SMPS_OK;
}

/**
  * @brief  Initialize the external SMPS component
  * @param  VoltageRange: Select operating SMPS supply
  *           @arg DCDC_AND_BOARD_DEPENDENT
  * @note   VoltageRange is not used with all boards. When not
  *         used, resort to PWR_REGULATOR_VOLTAGE_SCALE2 by default.
  * @retval SMPS status
  */
uint32_t BSP_SMPS_Init(uint32_t VoltageRange)
{
  PWR_AND_CLK_SMPS();

  GPIO_InitTypeDef GPIO_InitStruct;

  /* Reconfigure PWR_PUCRx/PDCRx registers only when not coming */
  /* back from Standby or Shutdown states.                      */
  /* Consider as well non-SMPS related pins.                     */
  if (!(READ_BIT(PWR->PWR_PU_REG, PWR_GPIO_ENABLE)))
  {
    HAL_PWREx_EnableGPIOPullDown(PWR_GPIO_SMPS, PWR_GPIO_SWITCH_ENABLE);
    HAL_PWREx_EnableGPIOPullDown(PWR_GPIO_SMPS, PWR_GPIO_ENABLE);

    /* HW limitation: Level shifter consumes because of dangling, so pull PA2 up
      (LPUART1_TX), PA13 (SWD/TMS) and PB3 (SWO) */
    HAL_PWREx_EnableGPIOPullUp(PWR_GPIO_A, GPIO_PIN_2); /* LPUART1_TX */
    HAL_PWREx_EnableGPIOPullUp(PWR_GPIO_A, GPIO_PIN_13); /* SWD/TMS    */
    HAL_PWREx_EnableGPIOPullUp(PWR_GPIO_B, GPIO_PIN_3); /* SWO        */

    /* Don't set PWR_CR3 APC bit at this time as it increases power
      consumption in non-Standby/Shutdown modes. It will have to be
      set with HAL_PWREx_EnablePullUpPullDownConfig() API upon
      Standby or Shutdown modes entering */
  }
  /* ------------------------------------------------------------------------ */
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Pull = GPIO_PULLUP;

  GPIO_InitStruct.Pin = PIN_SMPS_POWERGOOD;
  HAL_GPIO_Init(PORT_SMPS, &GPIO_InitStruct);

  /* ------------------------------------------------------------------------ */

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Pull = GPIO_NOPULL;

  GPIO_InitStruct.Pin = PIN_SMPS_ENABLE | PIN_SMPS_SWITCH_ENABLE;
  HAL_GPIO_Init(PORT_SMPS, &GPIO_InitStruct);

  /* --------- SMPS VOLTAGE RANGE SELECTION ----------------------------------*/
  /* ######################################################################## */
  /* - > Not applicable to ADP5301ACBZ on MB1319 */
  /* ######################################################################## */
  /* - > Applicable to ST1PS02D1QTR */
  /* Control to be added */

  /* ST1PS02D1QTR on MB1312 */
  /* if (VoltageRange == ST1PS02D1QTR_VOUT_1_25) */
  /* HAL_GPIO_WritePin(PORT_SMPS, PIN_SMPS_V1, GPIO_PIN_SET); */
  /* 1.25V                  */
  /* D0/D1/D2 = H/L/L       */
  /* else */

  /* */
  /* ST1PS02D1QTR on MB1312 */
  /* ST1PS02D1QTR_VOUT_1_05 */
  /* 1.05V                  */
  /* D0/D1/D2 = L/L/L       */
  /* HAL_GPIO_WritePin(PORT_SMPS, PIN_SMPS_V1, GPIO_PIN_RESET); */
  /* ######################################################################## */
  return SMPS_OK;
}

/**
  * @brief  Enable the external SMPS component
  * @param  Delay: delay in ms after enable
  * @param  Power_Good_Check: Enable Power good check
  * @note   Power_Good_Check is not used with all external
  *         SMPS components
  * @retval SMPS status
  *           @arg SMPS_OK: SMPS ENABLE OK
  *           @arg SMPS_KO: POWER GOOD CHECK FAILS
  */
uint32_t BSP_SMPS_Enable(uint32_t Delay, uint32_t Power_Good_Check)
{
  PWR_AND_CLK_SMPS();

  HAL_GPIO_WritePin(PORT_SMPS, PIN_SMPS_ENABLE, GPIO_PIN_SET);

  /* Delay upon request */
  if (Delay != 0)
  {
    HAL_Delay(Delay);
  }

  /* CHECK POWER GOOD or NOT */
  if (Power_Good_Check != 0)
  {
    if (GPIO_PIN_RESET == (HAL_GPIO_ReadPin(PORT_SMPS, PIN_SMPS_POWERGOOD)))
    {
      /* POWER GOOD KO */
      return SMPS_KO;
    }
  }

  /* SMPS ENABLED */
  return SMPS_OK;
}

/**
  * @brief  Disable the external SMPS component
  * @note   SMPS SWITCH should be disabled first !
  * @retval SMPS status
  *           @arg SMPS_OK: SMPS DISABLE OK - DONE
  *           @arg SMPS_KO: POWER GOOD CHECK FAILS
  *
  */
uint32_t BSP_SMPS_Disable(void)
{

  PWR_AND_CLK_SMPS();

  /* Check if SMPS SWITCH is disabled */
  if (HAL_GPIO_ReadPin(PORT_SMPS, PIN_SMPS_SWITCH_ENABLE) != GPIO_PIN_RESET)
  {
    /* ERROR AS SWITCH SHOULD BE DISABLED */
    return SMPS_KO;
  }

  /* Disable SMPS */
  HAL_GPIO_WritePin(PORT_SMPS, PIN_SMPS_ENABLE, GPIO_PIN_RESET);

  /* SMPS DISABLED */
  return SMPS_OK;
}

/**
  * @brief  Enable the external SMPS SWITCH component
  * @param  Delay: delay in ms before SMPS SWITCH ENABLE
  * @param  Power_Good_Check: Enable Power good check
  * @note   Power_Good_Check is not used with all boards
  * @retval SMPS status
  *           @arg SMPS_OK: SMPS ENABLE OK
  *           @arg SMPS_KO: POWER GOOD CHECK FAILS
  */
uint32_t BSP_SMPS_Supply_Enable(uint32_t Delay, uint32_t Power_Good_Check)
{
  PWR_AND_CLK_SMPS();

  if (Delay != 0)
  {
    HAL_Delay(Delay);
  }
  /* CHECK POWER GOOD or NOT */
  if (Power_Good_Check != 0)
  {
    if (GPIO_PIN_RESET == (HAL_GPIO_ReadPin(PORT_SMPS, PIN_SMPS_POWERGOOD)))
    {
      /* POWER GOOD KO */
      return SMPS_KO;
    }
  }

  /* SMPS SWITCH ENABLE */
  HAL_GPIO_WritePin(PORT_SMPS, PIN_SMPS_SWITCH_ENABLE, GPIO_PIN_SET);


  return SMPS_OK;
}

/**
  * @brief  Disable the external SMPS SWITCH component
  * @retval SMPS status
  *           @arg SMPS_OK: SMPS SWITCH DISABLE OK
  */
uint32_t BSP_SMPS_Supply_Disable(void)
{
  PWR_AND_CLK_SMPS();

  /* SMPS SWITCH DISABLE */
  HAL_GPIO_WritePin(PORT_SMPS, PIN_SMPS_SWITCH_ENABLE, GPIO_PIN_RESET);

  return SMPS_OK;
}

#endif /* ADP5301ACBZ */
#endif /* USE_STM32L4XX_NUCLEO_64_SMPS */

/**
  * @}
  */

/** @defgroup STM32L4XX_NUCLEO_Private_Functions Private Functions
  * @{
  */

#ifdef HAL_SPI_MODULE_ENABLED
/******************************************************************************
                            BUS OPERATIONS
*******************************************************************************/

#endif /* HAL_SPI_MODULE_ENABLED */

#ifdef HAL_ADC_MODULE_ENABLED




#endif /* HAL_ADC_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
