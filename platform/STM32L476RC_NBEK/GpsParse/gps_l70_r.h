//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : gps_l70_r.h
//  Date :     2018-1-15 22:11
//  Version :  V0001
//  History :  ��ʼ�����汾
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

//gps��λ��Ϣ����֪ͨ�ص�����Ҫ��APP��ص�
typedef void (*gps_notify_cb)(gps_type_e);

typedef void (*gps_init_cb)(void*cb);

//gps ��������Ҫ�Ĵ��ڷ��ͺ���������Ϣ�ϱ�����ָ��
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
  char utc[11];              //utcʱ�䡣hhmmss
  char state[2];             // A=��λ��V=δ��λ
  char latitude_value[10];   //γ��ֵ
  char latitude[2];          // N ��S
  char longitude_value[11];  //����ֵ��dddmm.mmmm
  char longitude[2];         // W �� E
  char speed[10];            //Speed over ground in knots
  char cog[7];               //��λ�ǣ���
  char date[7];              //���ڣ�DDMMYY
  char magnetic_angle[3];    //��ƫ��  һֱΪ��
  char magnetic_direction[2]; //��λ   һ��Ϊ��
  char mode[2];               //ģʽ��A = �Զ���D=��֣�E=���⣬N=��Ч
}gps_gprmc_t;

typedef gps_gprmc_t* GpsrmcHandle;

typedef struct
{
  double longtitude;  //����ֵ
  char   longtitude_char;
  double latitude;    //γ��ֵ 
  char   latitude_char;
  
}gps_info_t;

typedef gps_info_t* GpsHandle;

//******************************************************************************
// fn : GPS_Init
//
// brief : ʵʼ��GPSģ��
//
// param : cb->���»ص�
//
// return : none
extern void GPS_Init(void);

//******************************************************************************
// fn : GPS_Get
//
// brief : ��ȡGPS��λ��Ϣ
//
// param : none
//
// return : ��Ŷ�λ��Ϣ��ַ����Ч��λ��Ϣ������null
extern GpsrmcHandle GPS_Get(void);

//******************************************************************************
// fn : GPS_GetInfo
//
// brief : ��ȡGPS��λ��Ϣ
//
// param : none
//
// return : ��Ŷ�λ��Ϣ��ַ����Ч��λ��Ϣ������null
extern GpsHandle GPS_GetInfo(void);

//******************************************************************************
// fn : GPS_Standby
//
// brief : �������ģʽ
//
// param : none
//
// return : none
extern void GPS_Standby(void);

//******************************************************************************
// fn : GPS_SetNMEARMC
//
// brief : ����ֻ���RMC NMEA1803
//
// param : none
//
// return : !0-> ok
extern uint8_t GPS_SetNMEARMC(void);

//******************************************************************************
// fn : GPS_SetNMEADefault
//
// brief : ����NMEAĬ�������GPGSA,GPGGA,GPGSV,GPRMC,GPVTG,GPGLL
//
// param : none
//
// return : !0-> ok
//
extern uint8_t GPS_SetNMEADefault(void);

//******************************************************************************
// fn : GPS_HotStart
//
// brief : GPS ģ��������
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
