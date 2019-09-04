//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : gps_board_Cfg.c
//  Date     : 2018-11-20 15:47
//  Version  : V0001
// 历史记录  : 1.第一次创建
//
// 说明：
// NB_IOT与平台相关的配置变量定义
//******************************************************************************
#include "gps_board_cfg.h"
#include "los_bsp_gps.h"

extern void AppGpsMsg_cb(gps_type_e type);

gps_funcTable  gpsFunc = 
{
  .initFunc = LOS_EvbGpsUartITInit,
  .sentFunc = LOS_EvbGpsUartWrite,
  .notityCb = AppGpsMsg_cb
};
