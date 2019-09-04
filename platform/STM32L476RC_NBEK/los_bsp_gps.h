//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_bsp_gps.h
//  Date     : 2018-11-20 10:12
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#ifndef _LOS_BSP_GPS_H
#define _LOS_BSP_GPS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l4xx_hal.h"
#include "usart.h"

typedef struct
{
  const UART_HWAttrs* uartHWAttr;
  PIN_HWAttrs*        pwrPin;
}gps_hwattrs;

//*****************************************************************************
// fn : LOS_EvbGps_Init
//
// brief : Init the gps
//
// param : none
//
// return : none
extern void LOS_EvbGps_Init(void);

//******************************************************************************
// fn : LOS_EvbGpsUartITInit
//
// brief : Init the GPS Uart
//
// param : none
//
// return : none
extern void LOS_EvbGpsUartITInit(void*);

//******************************************************************************
// fn : LOS_EvbGpsUartWrite
//
// brief : 通过串口向外发送指定数据
//
// param : buf -> 数据缓存地址
//         len -> 数据长度
//
// return : none
extern void LOS_EvbGpsUartWrite(uint8_t *buf,uint16_t len);

//******************************************************************************
// fn : LOS_EvbGpsUartPoll
//
// brief : 轮询串口事件
//
// param : none
//
// return : 事件
extern uint8_t LOS_EvbGpsUartPoll(void);



#ifdef __cplusplus
}
#endif

#endif
