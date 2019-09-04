//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_led_entry.c
//  Date :     2018-11-13 11:45
//  Version :  V0001
//  History :  初始创建版本
//******************************************************************************
#include <string.h>

#include "los_led_entry.h"
#include "los_task.h"
#include "los_bsp_key.h"
#include "los_bsp_uart.h"

#include "display_cfg.h"

//------------------------------------------------------------------------------
#define LCD_FIRST_LINE       2
#define LCD_LAST_LINE        7

//------------------------------------------------------------------------------
//Global Variables Declare
static UINT32 g_uwLedTaskID;

//------------------------------------------------------------------------------
// function prototypes

/*  LED 任务函数入口 */
static LITE_OS_SEC_TEXT VOID LOS_BoardLedTskfunc(VOID);
/* 显示屏打印信息 */
static void LCD_Print(char *str,char *a);

//******************************************************************************
// fn : LOS_BoardLedEntry
//
// brief : 创建LED任务
//
// param : none
//
// return : none
void LOS_BoardLedEntry(void)
{
  UINT32 uwRet;
  TSK_INIT_PARAM_S stTaskInitParam;

  (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
  stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)LOS_BoardLedTskfunc;
  stTaskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
  stTaskInitParam.pcName = "LED-Demo";
  stTaskInitParam.usTaskPrio = 10;
  uwRet = LOS_TaskCreate(&g_uwLedTaskID, &stTaskInitParam);

  if (uwRet != LOS_OK)
  {
      return;
  }
  return;  
}
//******************************************************************************
// fn : LOS_BoardLedTskfunc
//
// brief : LED 任务函数入口
//
// param : none
//
// return : none
static LITE_OS_SEC_TEXT VOID LOS_BoardLedTskfunc(VOID)
{
  dispHandle = Display_open(0,NULL);
  Display_print0(dispHandle,1,0,4,"LiteOS");
  Display_print0(dispHandle,1,1,0,"Led Test");
  while (1)
  {
    LOS_EvbLedControl(LOS_LED1, LED_ON);
    LOS_EvbUartWriteStr("LED=ON\n");
    LCD_Print("LED=ON",NULL);
    (void)LOS_TaskDelay(500);
    LOS_EvbLedControl(LOS_LED1, LED_OFF);
    LOS_EvbUartWriteStr("LED=OFF\n");
    LCD_Print("LED=OFF",NULL);
    (void)LOS_TaskDelay(500);
  }
}
//******************************************************************************
// fn : LCD_Print 
//
// brief : 向屏打印信息
//
// param : str -> 字符串格式
//         a   -> 参数
// 
// return : none
void LCD_Print(char *str,char *a)
{ 
  static uint8_t line = LCD_FIRST_LINE;
  
  if(line > LCD_LAST_LINE)
  {
    Display_clearLines(dispHandle,LCD_FIRST_LINE,LCD_LAST_LINE);
    line = LCD_FIRST_LINE;
  }
  if(line < LCD_FIRST_LINE)
  {
    line = LCD_FIRST_LINE;
  }
  if(a == NULL)
  {
      Display_print0(dispHandle,1,line++,0,str);
  }
  else
  {
    Display_print1(dispHandle,1,line++,0,str,a);
  }
}
