//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_bsp_led.c
//  Date :     2018-10-29 10:40
//  Version :  V0001
//  History :  初始创建版本
//******************************************************************************
#include "los_bsp_led.h"

#ifdef LOS_STM32L476xx

#include "stm32l4xx_hal.h"
#include "stm32l4xx_nbek.h"
#include "gpio.h"
#endif
//--------------------------------------------------------------------------------
// Global variables

const PIN_HWAttrs  LED_HWAtrrs[LEDn] = {
  {
    LED0_PIN,
    LED0_GPIO_PORT,
    GPIO_MODE_OUTPUT_PP,
    GPIO_NOPULL,
    GPIO_PIN_RESET
  }
};

const PIN_Config ledCfg = {
  .num = LEDn,
  .hwAttrs = LED_HWAtrrs
};

//--------------------------------------------------------------------------------
// Function declear
//

/* Configure LED GPIO */
//static  void     BSP_LED_Init(Led_TypeDef Led);
/*DeInitialize LED GPIO*/
//static  void     BSP_LED_DeInit(Led_TypeDef Led);
/*Turn selected LED On*/
static  void     BSP_LED_On(Led_TypeDef Led);
/*Turn selected LED Off*/
static  void     BSP_LED_Off(Led_TypeDef Led);
/*Toggle the selected LED*/
static  void     BSP_LED_Toggle(Led_TypeDef Led);
//*********************************************************************************
// fn : LOS_EvbLedInit
//
// brief : Init the led
//
// param : none
//
// return : none
void LOS_EvbLedInit(void)
{

#ifdef LOS_STM32L476xx
  Board_GPIO_Init(&ledCfg);
#endif
    return;
}
//*********************************************************************************
// fn : LOS_EvbLedControl
//
// brief :control led on off
//
// param : index -> led's index
//         cmd   -> led on or off
//
// return : none
void LOS_EvbLedControl(int index, int cmd)
{
#ifdef LOS_STM32L476xx
  switch (index)
  {
      case LOS_LED1:
      {
          if (cmd == LED_ON)
          {
              BSP_LED_On(LED0);  /*led2 on */
          }
          else
          {
              BSP_LED_Off(LED0); /*led2 off */
          }
          break;
      }
      default:
      {
          break;
      }
  }
#endif
  return;
}
//***************************************************************************
// fn : BSP_LED_On
//
// brief : Turn selected LED On.
//
// param : led -> Specifies the Led to be set on.
//
// return : none
void BSP_LED_On(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_HWAtrrs[Led].Port, LED_HWAtrrs[Led].pin, GPIO_PIN_SET);
}

//***************************************************************************
// fn : BSP_LED_Off
//
// brief : Turn selected LED Off.
//
// param : led -> Specifies the Led to be set off.
//
// return : none
void BSP_LED_Off(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_HWAtrrs[Led].Port, LED_HWAtrrs[Led].pin, GPIO_PIN_RESET);
}
//***************************************************************************
// fn : BSP_LED_Toggle
//
// brief : Toggle the selected LED.
//
// param : led -> Specifies the Led to be toggled.
//
// return : none
void BSP_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(LED_HWAtrrs[Led].Port, LED_HWAtrrs[Led].pin);
}
