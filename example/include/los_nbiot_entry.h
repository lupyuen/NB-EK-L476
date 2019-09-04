//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_nbiot_entry.h
//  Date     : 2018-11-13 22:43
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#ifndef _LOS_NBIOT_ENTRY_H
#define _LOS_NBIOT_ENTRY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32l4xx_hal.h"

//******************************************************************************
// fn : LOS_NBIOT_Entry
//
// brief : 创建nbiot系统任务
//
// param : none
//
// return : none
extern void LOS_NBIOT_Entry(void);


#ifdef __cplusplus
}
#endif

#endif  //_LOS_NBIOT_ENTRY_H
