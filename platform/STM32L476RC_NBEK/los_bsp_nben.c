//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_bsp_nben.c
//  Date :     2018-11-13 15:48
//  Version :  V0001
//  History :  初始创建版本
//******************************************************************************
#include "los_bsp_nben.h"

#ifdef LOS_STM32L476xx

#include "gpio.h"
#endif

const PIN_HWAttrs  NBEN_HWAtrrs = {
  NB_PWR_PIN,
  NB_PWR_PORT,
  GPIO_MODE_OUTPUT_PP,
  GPIO_NOPULL,
  0,
  GPIO_PIN_SET
};

//*******************************************************************************
// fn : LOS_EvbNBPwrInit
//
// brief : 初始化en引脚
//
// param : none
//
// return : none
extern void LOS_EvbNBPwrInit(void)
{
  Board_GPIO_OneInit(&NBEN_HWAtrrs);
}

//*******************************************************************************
// fn : LOS_EvbNBPwrOnOff
//
// brief : 初始化en引脚
//
// param : opt -> 0 : disable; others enable
//
// return : none
extern void LOS_EvbNBPwrOnOff(char opt)
{
  GPIO_PinState state;
  if(opt)
  {
    state =  GPIO_PIN_SET;
  }
  else
  {
    state =  GPIO_PIN_RESET;    
  }
  HAL_GPIO_WritePin(NBEN_HWAtrrs.Port, NBEN_HWAtrrs.pin, state);
}
