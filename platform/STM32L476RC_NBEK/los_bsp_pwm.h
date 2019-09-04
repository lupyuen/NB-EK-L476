//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_bsp_pwm.h
//  Date     : 2018-11-15 22:36
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#ifndef _LOS_BSP_PWM_H
#define _LOS_BSP_PWM_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "stm32l4xx_hal.h"
#include "lptim.h"
	

//*********************************************************************************
// fn : LOS_EvbPwm_Init
//
// brief : Init the Light
//
// param : none
//
// return : none
extern void LOS_EvbPwm_Init(void);
	
//*********************************************************************************
// fn : LOS_EvbPwm_Set
//
// brief : Set the duty
//
// param : none
//
// return : none
extern void LOS_EvbPwm_Set(uint8_t);
	
#ifdef __cplusplus
}
#endif	
#endif  //_LOS_BSP_PWM_H
