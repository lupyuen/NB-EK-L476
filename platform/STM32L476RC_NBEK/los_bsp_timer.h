//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_bsp_timer.h
//  Date     : 2018-11-12 22:42
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#ifndef _LOS_BSP_TIMER_H
#define _LOS_BSP_TIMER_H

#ifdef __cplusplus
extern "C" 
{
#endif

//------------------------------------------------------------------------------
// Include
#include "nb_bc95.h"
	
//------------------------------------------------------------------------------
// Functions prototypes

//***********************************************************************
// fn : LOS_EvbTimer_Init
//
// brief : Init a software timer
//
// param: cb -> timeout cb
//
// return : none
extern void LOS_EvbTimer_Init(bc95_timeout_cb cb);
	
//***********************************************************************
// fn : LOS_EvbTimer_Start
//
// brief : Start a software timer
//
// param: ms -> period 
//
// return : none
extern void LOS_EvbTimer_Start(uint32_t ms);
//***********************************************************************
// fn : LOS_EvbTimer_Stop
//
// brief : Stop a software timer
//
// param: none
//
// return : none
extern void LOS_EvbTimer_Stop(void);
	
	
#ifdef __cplusplus
}
#endif
#endif   //_LOS_BSP_TIMER_H
