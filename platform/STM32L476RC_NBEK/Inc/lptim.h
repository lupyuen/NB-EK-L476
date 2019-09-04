//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : lptim.h
//  Date     : 2018-11-15 22:10
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#ifndef _LPTIM_H
#define _LPTIM_H

#ifdef __cplusplus
extern "C" {
#endif
	 
/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nbek.h"
#include "gpio.h"
	
typedef struct
{
	const PIN_HWAttrs   *pTimpin;
	LPTIM_HandleTypeDef *hlptim1;
}LPTIM_HWAttrs;

typedef struct
{
	const LPTIM_HWAttrs *phwattrs;

}LPTIM_Config;

//*************************************************************************
// fn : Board_LPTIM_Init
//
// brief : Init the peripheral
//
// param : cfg -> the reference of LPTIM_Config obj
//
// return : HAL_ERROR or HAL_OK
extern uint32_t Board_LPTIM_Init(const LPTIM_Config* cfg);

//*************************************************************************
// fn : Board_LPTIM_Close
//
// brief : Init the peripheral
//
// param : cfg -> the reference of LPTIM_Config obj
//
// return : HAL_ERROR or HAL_OK
extern uint32_t Board_LPTIM_Close(const LPTIM_Config* cfg);


//*************************************************************************
// fn : Board_LPTIM_PWM
//
// brief : Init the peripheral
//
// param : cfg -> the reference of LPTIM_Config obj
//         duty->
//
// return : HAL_ERROR or HAL_OK
extern uint32_t Board_LPTIM_PWM(const LPTIM_Config* cfg,uint8_t duty);
	 
#ifdef __cplusplus
}
#endif

#endif   //_LPTIM_H
