//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_nbreport.h
//  Date     : 2018-11-26 22:00
//  Version  : V0001
// ��ʷ��¼  : ��һ�δ���
// ����������ʪ���ϱ��Ľṹ�壬���������ݴ��ģʽ�任
//******************************************************************************
#ifndef _LOS_NBREPORT_H
#define _LOS_NBREPORT_H

#ifdef __cplusplus
extern "C"
{
#endif
	
#include "nb_types.h"
	
//��ʪ�������ϱ�
typedef struct
{
  const uint8 msgId;     //msgId
  int16 temp;            //�¶�
  uint8 humi;            //ʪ��
}th_info_t,*pTH;

extern th_info_t th_info;

typedef struct
{
  uint8   endian;       //��¼��ǰcpu�Ĵ�С��ģʽ
}cpu_info_t;

extern cpu_info_t cpu_info;

#define REPORT_PERIOD_MIN    5
typedef struct
{
  uint32 period;       //Ĭ��5s,��С5s,����ͨ��KEY_LEFT��KEY_RIGHT��������
  uint8  isSend;       //0���ϱ���1�ϱ���ʪ������
}send_cfg_t;

extern send_cfg_t send_cfg;	
	
//******************************************************************************
// fn : getCpuEndian
//
// brief : ��ȡCPU��С��ģʽ
//
// param : none
//
// return : 0->little ; other-> big
uint8 getCpuEndian(void);

//******************************************************************************
// fn : ConvertNetData
//
// brief : ����������ת�ɴ��ģʽ����������
//
// param : s -> Դ���ݵ�ַ
//         t -> Ŀ���ַ
//
// return : the length
uint8 ConvertNetData(pTH s, char*t);

#ifdef __cplusplus
}
#endif	
	
#endif //_LOS_NBREPORT_H
