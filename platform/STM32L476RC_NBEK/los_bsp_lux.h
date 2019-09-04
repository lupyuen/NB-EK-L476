//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_bsp_lux.h
//  Date     : 2018-11-15 11:36
//  Version  : V0001
//  History Record : The first
//  Description : 
//******************************************************************************
#ifndef _LOS_BSP_LUX_H
#define _LOS_BSP_LUX_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32l4xx_hal.h"

//******************************************************************************
// fn : LOS_EvbLux_Init
//
// brief : init the adc
//
// param : none
//
// return : none
extern void LOS_EvbLux_Init(void);


//******************************************************************************
// fn : LOS_EvbLux_Read
//
// brief : Read lux 
//
// param : none
//
// return : none
extern uint16_t LOS_EvbLux_Read(void);

//******************************************************************************
// fn : LOS_EvbLux_Read
//
// brief : adc to lux
//
// param : adc ->  adc value
//
// return : lux
extern uint16_t LOS_EvbLux_Convert(uint16_t adc);
#ifdef __cplusplus
}
#endif

#endif //_LOS_BSP_LUX_H
