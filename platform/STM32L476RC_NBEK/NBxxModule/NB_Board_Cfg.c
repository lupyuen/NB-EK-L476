//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : NB_Board_Cfg.c
//  Date     : 2017-12-21 5:45
//  Version  : V0001
// 历史记录  : 1.第一次创建
//
// 说明：
// NB_IOT与平台相关的配置变量定义
//******************************************************************************
#include "los_bsp_nbuart.h"
#include "los_bsp_timer.h"
#include "los_bsp_nben.h"

#include "NB_Board.h"
#include "NB_BC95.h"

extern int NB_MsgreportCb(msg_types_t,int ,char*);

//===============================================================================
const NB_Pwr nbPwr_fxn = 
{
  .nbPwrInit = LOS_EvbNBPwrInit,
  .nbPwrOnOff = LOS_EvbNBPwrOnOff
};

const com_fxnTable  com_fxn = 
{
  .openFxn = LOS_EvbNBUartDMAInit,
  .sendFxn = LOS_EvbNBUartWrite,
  .closeFxn = LOS_EvbNBUartClose
};

const time_fxnTable time_fxn = 
{
  .initTimerFxn  = LOS_EvbTimer_Init,
  .startTimerFxn = LOS_EvbTimer_Start,
  .stopTimerFxn  = LOS_EvbTimer_Stop
};

const hw_object_t  HWAtrrs_object = 
{
  .baudrate = 9600,
  .uart_fxnTable = &com_fxn,
  .timer_fxnTable = &time_fxn
};

NB_Config  nb_config = 
{
  .fxnTablePtr = NULL,
  .object = (void*)&HWAtrrs_object,
  .AppReceCB = NB_MsgreportCb,
  .nbPwr = &nbPwr_fxn,
  .log = NULL
};
