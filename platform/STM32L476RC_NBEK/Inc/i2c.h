//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : i2c.h
//  Date     : 2018-11-14 11:10
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#ifndef __I2C_H
#define __I2C_H

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nbek.h"
#include "gpio.h"

typedef struct
{
  PIN_HWAttrs sclPin;
  PIN_HWAttrs sdaPin;
}I2C_HWAttrs;

typedef struct
{
  I2C_HandleTypeDef  *pI2c;
  const I2C_HWAttrs  *hwAttrs;
}I2C_Config;

//****************************************************************************
// fn : Board_I2C_Init
//
// brief : init a i2c periphral
//
// param : i2cCfg -> the reference of i2c_config
//
// return : HAL_OK or HAL_ERROR
extern uint32_t Board_I2C_Init(const I2C_Config* i2cCfg);

//****************************************************************************
// fn : Board_I2C_Send
//
// brief : Send the data
//
// param : i2cCfg -> the reference of i2c_config
//         addr ->  the slave addr
//         pdata -> the pointer of data
//         len -> the length
//
// return : HAL_OK or HAL_ERROR
extern uint32_t Board_I2C_Send(const I2C_Config* i2cCfg,uint8_t addr , uint8_t *pData, uint16_t len);

//****************************************************************************
// fn : Board_I2C_Read
//
// brief : Rend the data
//
// param : i2cCfg -> the reference of i2c_config
//         addr ->  the slave addr
//         pdata -> the pointer of data
//         len -> the length
//
// return : HAL_OK or HAL_ERROR
extern uint32_t Board_I2C_Rece(const I2C_Config* i2cCfg,uint8_t addr , uint8_t *pData, uint16_t len);

#ifdef __cplusplus
}
#endif
#endif //__I2C_H
