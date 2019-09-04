//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : Displsy_cfg.c
//  Date     : 2018-10-23 11:25
//  Version  : V0001
// 历史记录  : 1.第一次创建
//
// 说明：
// 显示屏配置变量定义
//******************************************************************************
#include "stm32l4xx_nbek.h"
#include "Display.h"
#include "DisplayTFT128.h"
#include "TFT128.h"

//-------------------------------------------------------------------------------
// extern variable declare
//
extern const LCD_Config LCD_config;

//-------------------------------------------------------------------------------
// global variable 
//

/* LCD Handle*/
Display_Handle  dispHandle;

/* Structures for tft1.44 */
DisplayTFT128_Object  displayTft128Object;


const DisplayTFT128_HWAttrs displayTft128HWattrs = {
    .lcdHandle = (LCD_Handle)& LCD_config,
    .powerPin  = {LCD_BL_PIN,LCD_BL_GPIO_PORT,GPIO_MODE_OUTPUT_PP,GPIO_PULLUP,GPIO_PIN_RESET}
};

/* Array of displays */
const Display_Config Display_config[] = 
{
  {
    .fxnTablePtr = &DisplayTFT128_fxnTable,
    .object      = &displayTft128Object,
    .hwAttrs     = &displayTft128HWattrs  
  },

  { NULL, NULL, NULL } // Terminator
};
