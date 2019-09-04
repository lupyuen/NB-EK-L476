//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_led_entry.h
//  Date :     2018-11-13 11:45
//  Version :  V0001
//  History :  初始创建版本
//******************************************************************************
#ifndef _LOS_LED_ENTRY_H
#define _LOS_LED_ENTRY_H

#ifdef __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
// Include
#include "los_bsp_led.h"

//------------------------------------------------------------------------------
//Functions prototypes

//******************************************************************************
// fn : LOS_BoardLedEntry
//
// brief : 创建LED任务
//
// param : none
//
// return : none
extern void LOS_BoardLedEntry(void);

#ifdef __cplusplus
}
#endif

#endif  //_LOS_LED_ENTRY_H
