//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : adc.h
//  Date     : 2018-11-15 10:36
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#ifndef _ADC_H
#define _ADC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32l4xx_hal.h"
#include "gpio.h"


#define ADC_MODE_BLOCK     0x0001
#define ADC_MODE_IT        0x0002
#define ADC_MODE_DMA       0x0004


//------------------------------------------------------------------------------
//Typedef define

typedef struct
{
  const PIN_HWAttrs  *adcPin;
  ADC_HandleTypeDef  *pAdc;
  
}ADC_HWAttrs;

typedef struct
{
  uint32_t muxId;
  uint32_t timeout;
  uint32_t mode;
}ADC_Object;

typedef struct
{
  const ADC_HWAttrs            *phwattrs;
  const ADC_MultiModeTypeDef   *pMode;
  const ADC_ChannelConfTypeDef *pChCfg;
  ADC_Object                   *pOjb;
}ADC_Config;

//********************************************************************************
// fn : Board_ADC_Init
//
// brief : Init the adc periphral
//
// param : adcCfg -> the reference of ADC_Config
//
// return : HAL_OK or HAL_ERROR
extern uint32_t Board_ADC_Init(const ADC_Config* adcCfg);


//********************************************************************************
// fn : Board_ADC_Read
//
// brief : Read a value from adc
//
// param : adcCfg -> the reference of ADC_Config
//
// return : value
extern uint16_t Board_ADC_Read(const ADC_Config* adcCfg);


//********************************************************************************
// fn : Board_ADC_Close
//
// brief : Close a value from adc
//
// param : adcCfg -> the reference of ADC_Config
//
// return : value
extern uint16_t Board_ADC_Close(const ADC_Config* adcCfg);

#ifdef __cplusplus
}
#endif

#endif        //_ADC_H
