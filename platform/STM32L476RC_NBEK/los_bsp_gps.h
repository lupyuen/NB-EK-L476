//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_bsp_gps.h
//  Date     : 2018-11-20 10:12
//  Version  : V0001
// ��ʷ��¼  : ��һ�δ���
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
// brief : ͨ���������ⷢ��ָ������
//
// param : buf -> ���ݻ����ַ
//         len -> ���ݳ���
//
// return : none
extern void LOS_EvbGpsUartWrite(uint8_t *buf,uint16_t len);

//******************************************************************************
// fn : LOS_EvbGpsUartPoll
//
// brief : ��ѯ�����¼�
//
// param : none
//
// return : �¼�
extern uint8_t LOS_EvbGpsUartPoll(void);



#ifdef __cplusplus
}
#endif

#endif
