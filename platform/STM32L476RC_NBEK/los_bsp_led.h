//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_bsp_led.h
//  Date :     2018-10-29 10:36
//  Version :  V0001
//  History :  初始创建版本
//******************************************************************************
#ifndef _LOS_BSP_LED_H
#define _LOS_BSP_LED_H

#ifdef __cplusplus
extern "C"
{
#endif

#define LED_ON      (1)
#define LED_OFF     (0)
#define LOS_LED1    (1)

//---------------------------------------------------------------------------------
//*********************************************************************************
// fn : LOS_EvbLedInit
//
// brief : Init the led
//
// param : none
//
// return : none
void LOS_EvbLedInit(void);
//*********************************************************************************
// fn : LOS_EvbLedControl
//
// brief :control led on off
//
// param : index -> led's index
//         cmd   -> led on or off
//
// return : none
void LOS_EvbLedControl(int index, int cmd);

#ifdef __cplusplus
}
#endif
#endif
