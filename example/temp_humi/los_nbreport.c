//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_nbreport.c
//  Date     : 2018-11-26 22:00
//  Version  : V0001
// ��ʷ��¼  : ��һ�δ���
// ����������ʪ���ϱ��Ľṹ�壬���������ݴ��ģʽ�任
//******************************************************************************
#include "los_nbreport.h"

//******************************************************************************
// Global variables

th_info_t th_info   = {0,0,0};   //������ʪ�ȸ�ʽ����
cpu_info_t cpu_info = {0};       //����cpu��С�˽ṹ����
send_cfg_t send_cfg = {5,TRUE};	 //�����ϱ����ݿ��Ʊ���

//******************************************************************************
// fn : getCpuEndian
//
// brief : ��ȡCPU��С��ģʽ
//
// param : none
//
// return : 0->little ; other-> big
uint8 getCpuEndian(void)
{
  union endian
  {
    int a;
    char b;
  }tmp;
  tmp.a = 1;
  return(tmp.b != 1);
}
//******************************************************************************
// fn : ConvertNetData
//
// brief : ����������ת�ɴ��ģʽ����������
//
// param : s -> Դ���ݵ�ַ
//         t -> Ŀ���ַ
//
// return : the length
uint8 ConvertNetData(pTH s, char*t)
{
  uint8 len = 0;
  
  if(s == NULL || t == NULL)
  {
    return len;
  }
  if(cpu_info.endian)  
  {
    //????
    *t++ = s->msgId;
    *t++ = *((uint8*)&s->temp);
    *t++ = *(((uint8*)&s->temp)+1);
    *t++ = s->humi;
  }
  else
  {
    *t++ = s->msgId;
    *t++ = *(((uint8*)&s->temp)+1);
    *t++ = *((uint8*)&s->temp);
    *t++ = s->humi;
  }
  len = 4;
  return len;
}
