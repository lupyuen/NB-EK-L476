//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_nbreport.c
//  Date     : 2018-11-26 22:00
//  Version  : V0001
// 历史记录  : 第一次创建
// 定义输助温湿度上报的结构体，与网络数据大端模式变换
//******************************************************************************
#include "los_nbreport.h"

//******************************************************************************
// Global variables

th_info_t th_info   = {0,0,0};   //定议温湿度格式变量
cpu_info_t cpu_info = {0};       //定义cpu大小端结构变量
send_cfg_t send_cfg = {5,TRUE};	 //定义上报数据控制变量

//******************************************************************************
// fn : getCpuEndian
//
// brief : 获取CPU大小端模式
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
// brief : 将本地数据转成大端模式的网络数据
//
// param : s -> 源数据地址
//         t -> 目标地址
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
