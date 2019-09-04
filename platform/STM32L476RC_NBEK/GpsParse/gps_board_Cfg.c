//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : gps_board_Cfg.c
//  Date     : 2018-11-20 15:47
//  Version  : V0001
// ��ʷ��¼  : 1.��һ�δ���
//
// ˵����
// NB_IOT��ƽ̨��ص����ñ�������
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
