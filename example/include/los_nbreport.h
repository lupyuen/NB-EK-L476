//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_nbreport.h
//  Date     : 2018-11-26 22:00
//  Version  : V0001
// 历史记录  : 第一次创建
// 定义输助温湿度上报的结构体，与网络数据大端模式变换
//******************************************************************************
#ifndef _LOS_NBREPORT_H
#define _LOS_NBREPORT_H

#ifdef __cplusplus
extern "C"
{
#endif
	
#include "nb_types.h"
	
//温湿度数据上报
typedef struct
{
  const uint8 msgId;     //msgId
  int16 temp;            //温度
  uint8 humi;            //湿度
}th_info_t,*pTH;

extern th_info_t th_info;

typedef struct
{
  uint8   endian;       //记录当前cpu的大小端模式
}cpu_info_t;

extern cpu_info_t cpu_info;

#define REPORT_PERIOD_MIN    5
typedef struct
{
  uint32 period;       //默认5s,最小5s,可以通过KEY_LEFT与KEY_RIGHT进行配置
  uint8  isSend;       //0不上报，1上报温湿度数据
}send_cfg_t;

extern send_cfg_t send_cfg;	
	
//******************************************************************************
// fn : getCpuEndian
//
// brief : 获取CPU大小端模式
//
// param : none
//
// return : 0->little ; other-> big
uint8 getCpuEndian(void);

//******************************************************************************
// fn : ConvertNetData
//
// brief : 将本地数据转成大端模式的网络数据
//
// param : s -> 源数据地址
//         t -> 目标地址
//
// return : the length
uint8 ConvertNetData(pTH s, char*t);

#ifdef __cplusplus
}
#endif	
	
#endif //_LOS_NBREPORT_H
