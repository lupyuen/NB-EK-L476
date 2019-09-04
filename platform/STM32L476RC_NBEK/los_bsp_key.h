//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_bsp_key.h
//  Date :     2018-1-09 10:40
//  Version :  V0001
//  History :  初始创建版本
//******************************************************************************
#ifndef _LOS_BSP_KEY_H
#define _LOS_BSP_KEY_H

#ifdef __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
// Include Area
//------------------------------------------------------------------------------
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nbek.h"

#define  LOS_KEY_PRESS    0
#define  USER_KEY         1
#define  KEY_DELAY_TICK   20

typedef struct
{
  Button_TypeDef   key;
  uint8_t          flag;    //true of false
	
}key_press_t;

//---------------------------------------------------------------------------------
//******************************************************************************
// fn : LOS_EvbKeyInit
//
// brief : Init the button
//
// param : none
//
// return : none
extern void LOS_EvbKeyInit(void);

//******************************************************************************
// fn : LOS_EvbGetKey
//
// brief : Get the key 
//
// param : tickout ->  read timeout .unit is system tick
//
// return : BTN_UP,BTN_RIGHT,BTN_DOWN,BTN_LEFT
extern Button_TypeDef LOS_EvbGetKey(uint8_t tickout);

#ifdef __cplusplus
}
#endif

#endif
