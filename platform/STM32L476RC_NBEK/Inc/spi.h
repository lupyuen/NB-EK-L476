//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : spi.h
//  Date     : 2018-11-05 11:15
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#ifndef _SPI_H
#define _SPI_H

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "gpio.h"

typedef struct
{
  SPI_TypeDef*  spiIndex;
  uint32_t      clk;
  uint32_t      mode;
  uint32_t      dataSize;
  uint32_t      polarity;
  uint32_t      phase;
}SPI_Param_t;

typedef struct
{
  PIN_HWAttrs const* sckPin;
  PIN_HWAttrs const* misoPin;
  PIN_HWAttrs const* mosiPin;
  PIN_HWAttrs const* csPin;
}SPI_HWAttrs;

typedef struct
{
  SPI_HandleTypeDef*  spiHandle; 
  SPI_HWAttrs const*  spiHWAtrrs;
  
}SPI_Config;

//****************************************************************************
// fn : Board_SPI_Init
//
// brief : Init the spi perpheral
//
// param : spiCfg -> the spi configuration
//
// return : success or fail
extern uint32_t Board_SPI_Init(const SPI_Config* spiCfg);

//****************************************************************************
// fn : Board_SPI_DeInit
//
// brief : Deinit the spi perpheral
//
// param : spiCfg -> the spi configuration
//
// return : success or fail
extern uint32_t Board_SPI_DeInit(const SPI_Config* spiCfg);

#ifdef __cplusplus
}
#endif

#endif
