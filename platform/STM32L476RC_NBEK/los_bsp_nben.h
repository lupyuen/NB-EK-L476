//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_bsp_nben.h
//  Date :     2018-11-13 10:48
//  Version :  V0001
//  History :  ��ʼ�����汾
//******************************************************************************
#ifndef _LOS_BSP_NBEN_H
#define _LOS_BSP_NBEN_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l4xx_hal.h"
#include "stm32l4xx_nbek.h"

//*******************************************************************************
// fn : LOS_EvbNBPwrInit
//
// brief : ��ʼ��en����
//
// param : none
//
// return : none
extern void LOS_EvbNBPwrInit(void);

//*******************************************************************************
// fn : LOS_EvbNBPwrOnOff
//
// brief : ��ʼ��en����
//
// param : opt -> 0 : disable; others enable
//
// return : none
extern void LOS_EvbNBPwrOnOff(char opt);

#ifdef __cplusplus
}
#endif

#endif
