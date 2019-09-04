//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_bsp_timer.c
//  Date     : 2018-11-12 22:43
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#include "los_bsp_timer.h"
#include "los_swtmr.h"

//------------------------------------------------------------------------------
// Variables declare
//
bc95_timeout_cb  nb_timeoutCb;
UINT16           nb_TimeId;
//------------------------------------------------------------------------------
// Functions prototypes
//
static void Timer_Callback    (UINT32 arg);   // callback fuction
//***********************************************************************
// fn : LOS_EvbTimer_Init
//
// brief : Init a software timer
//
// param: cb -> timeout cb
//
// return : none
void LOS_EvbTimer_Init(bc95_timeout_cb cb)
{
  nb_timeoutCb = cb;
}
	
//***********************************************************************
// fn : LOS_EvbTimer_Start
//
// brief : Start a software timer
//
// param: ms -> period 
//
// return : none
void LOS_EvbTimer_Start(uint32_t ms)
{
  UINT32 uwRet = LOS_OK;
  //创建单次触发软定时器
  //注：执行完超时回调后，系统将定时器自动删除
  uwRet = LOS_SwtmrCreate(LOS_MS2Tick(ms), LOS_SWTMR_MODE_ONCE,Timer_Callback,&nb_TimeId,1);
  if(LOS_OK != uwRet)
  {
#ifdef UART_DBG
      printf("create nbTimer failed\n");
#endif
  }
  else
  {
#ifdef UART_DBG
      printf("create nbTimer success\n");
#endif
  }
  LOS_SwtmrStart(nb_TimeId);

}
//***********************************************************************
// fn : LOS_EvbTimer_Stop
//
// brief : Stop a software timer
//
// param: none
//
// return : none
void LOS_EvbTimer_Stop(void)
{
	LOS_SwtmrStop(nb_TimeId);
  LOS_SwtmrDelete(nb_TimeId);
}
//**********************************************************************
// fn : Timer_Callback
//
// brief : 软定时器超时回调函数
//
// param : arg -> 回调参数
//
// return : none
// ACTION:
// 此函数执行完之后，系统将自动删除创建的软定时器
void Timer_Callback(UINT32 arg)
{
  if(nb_timeoutCb)
  {
    nb_timeoutCb();
  }
}
