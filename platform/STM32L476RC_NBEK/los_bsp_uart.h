//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_bsp_uart.h
//  Date     : 2018-10-28 14:05
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************

#ifndef _LOS_BSP_UART_H
#define _LOS_BSP_UART_H

#ifdef LOS_STM32L476xx

#include "stm32l4xx_hal.h"

#endif
//************************************************************************************
// fn : LOS_EvbUartInit
//
// brief : Init the log uart 
//
// param : none
//
// return : none
extern void LOS_EvbUartInit(void);
//***************************************************************************************
// fn : LOS_EvbUartWriteByte
//
// brief : send one byte with uart
//
// param : c -> output
//
// return : none
extern void LOS_EvbUartWriteByte(char c);
//***************************************************************************************
// fn : LOS_EvbUartWriteStr
//
// brief : send strings with uart
//
// param : str -> the reference of the output 
//
// return : none
extern void LOS_EvbUartWriteStr(const char* str);

#endif
