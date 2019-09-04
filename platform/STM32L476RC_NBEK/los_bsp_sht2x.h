//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_bsp_sht2x.h
//  Date     : 2018-11-14 11:47
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#ifndef _LOS_BSP_SHT2x_H
#define _LOS_BSP_SHT2x_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l4xx_hal.h"
#include "i2c.h"
	
typedef struct
{
	const I2C_Config* pI2cCfg;
	uint32_t          semId;
}SHT2x_Handle;
//*********************************************************************************
// fn : LOS_EvbSHT2x_Init
//
// brief : Init the sht2x
//
// param : none
//
// return : none
extern void LOS_EvbSHT2x_Init(void);

//******************************************************************************
// fn : SHT20_ReadTemp
//
// brief : read tempture from sht20 with iic
//
// param : none
//
// return : none
extern uint16_t SHT20_ReadTemp(void);

//******************************************************************************
// fn : SHT20_ReadRH
//
// brief : read RH from sht20 with iic
//
// param : none
//
// return : none
extern uint16_t SHT20_ReadRH(void);

//******************************************************************************
// fn : SHT20_SoftReset
//
// brief : softreset sht20
//
// param : none
//
// return : none
extern void SHT20_SoftReset(void);

//******************************************************************************
// fn : SHT20_Convert
//
// brief : 将原始的AD值转成相应的温度值与湿度值
//
// param : value-> ad值
//         isTemp-> >0 温度值，=0 湿度值
//
// return : 直观的温度值
extern float SHT20_Convert(uint16_t value,uint8_t isTemp);

#ifdef __cplusplus
}
#endif

#endif //_LOS_BSP_SHT2x_H
