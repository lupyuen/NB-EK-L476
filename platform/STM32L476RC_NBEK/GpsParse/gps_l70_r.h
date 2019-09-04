//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : gps_l70_r.h
//  Date :     2018-1-15 22:11
//  Version :  V0001
//  History :  初始创建版本
//******************************************************************************
#ifndef _GPS_L70_R_H
#define _GPS_L70_R_H

#ifdef __cplusplus
extern "C"
{
#endif
  
#include <stdint.h>
  
typedef enum
{
  GPS_TYPE_RMC
}gps_type_e;
  
typedef void (*gps_receive_cb)(uint8_t*, uint16_t);
typedef gps_receive_cb gps_send_func;

//gps定位信息更新通知回调。主要向APP层回调
typedef void (*gps_notify_cb)(gps_type_e);

typedef void (*gps_init_cb)(void*cb);

//gps 操作所需要的串口发送函数，和消息上报函数指针
typedef struct
{
  gps_init_cb    initFunc;
  gps_send_func  sentFunc;
  gps_notify_cb  notityCb;
}gps_funcTable;

//******************************************************************************
// typedef 
//
typedef struct
{
  char nmea_id[6];           //nmea message id
  char utc[11];              //utc时间。hhmmss
  char state[2];             // A=定位，V=未定位
  char latitude_value[10];   //纬度值
  char latitude[2];          // N 或S
  char longitude_value[11];  //经度值。dddmm.mmmm
  char longitude[2];         // W 或 E
  char speed[10];            //Speed over ground in knots
  char cog[7];               //方位角，度
  char date[7];              //日期，DDMMYY
  char magnetic_angle[3];    //磁偏角  一直为空
  char magnetic_direction[2]; //方位   一真为空
  char mode[2];               //模式。A = 自动，D=差分，E=估测，N=无效
}gps_gprmc_t;

typedef gps_gprmc_t* GpsrmcHandle;

typedef struct
{
  double longtitude;  //经度值
  char   longtitude_char;
  double latitude;    //纬度值 
  char   latitude_char;
  
}gps_info_t;

typedef gps_info_t* GpsHandle;

//******************************************************************************
// fn : GPS_Init
//
// brief : 实始化GPS模块
//
// param : cb->更新回调
//
// return : none
extern void GPS_Init(void);

//******************************************************************************
// fn : GPS_Get
//
// brief : 获取GPS定位信息
//
// param : none
//
// return : 存放定位信息地址。无效定位信息将返回null
extern GpsrmcHandle GPS_Get(void);

//******************************************************************************
// fn : GPS_GetInfo
//
// brief : 获取GPS定位信息
//
// param : none
//
// return : 存放定位信息地址。无效定位信息将返回null
extern GpsHandle GPS_GetInfo(void);

//******************************************************************************
// fn : GPS_Standby
//
// brief : 进入待机模式
//
// param : none
//
// return : none
extern void GPS_Standby(void);

//******************************************************************************
// fn : GPS_SetNMEARMC
//
// brief : 设置只输出RMC NMEA1803
//
// param : none
//
// return : !0-> ok
extern uint8_t GPS_SetNMEARMC(void);

//******************************************************************************
// fn : GPS_SetNMEADefault
//
// brief : 设置NMEA默认输出：GPGSA,GPGGA,GPGSV,GPRMC,GPVTG,GPGLL
//
// param : none
//
// return : !0-> ok
//
extern uint8_t GPS_SetNMEADefault(void);

//******************************************************************************
// fn : GPS_HotStart
//
// brief : GPS 模块热启动
//
// param : none
//
// return : !0-> ok
extern uint8_t GPS_HotStart(void);

void GPS_handle_msg(char*msg, uint16_t len);
#ifdef __cplusplus
}
#endif
#endif   //_GPS_L70_R_H
