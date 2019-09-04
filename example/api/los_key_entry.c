//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_led_entry.c
//  Date :     2018-11-14 09:43
//  Version :  V0001
//  History :  初始创建版本
//******************************************************************************
#include <string.h>
#include "los_key_entry.h"
#include "los_task.h"

#include "los_bsp_led.h"
#include "los_bsp_key.h"
#include "los_bsp_uart.h"

#include "display_cfg.h"

//------------------------------------------------------------------------------
#define LCD_FIRST_LINE       2
#define LCD_LAST_LINE        7

//------------------------------------------------------------------------------
//  Global Variables Declare
static UINT32 g_uwKeyTaskID;

//------------------------------------------------------------------------------
// function prototypes

/*  key 任务函数入口 */

static LITE_OS_SEC_TEXT VOID LOS_BoardKeyTskfunc(VOID);
/* 显示屏打印信息 */
static void LCD_Print(char *str,char *a);
//------------------------------------------------------------------------------
//******************************************************************************
// fn : LOS_BoardKey_Entry
//
// brief : 创建一个按钮测试任务
//
// param : none
//
// return : none
extern void LOS_BoardKey_Entry(void)
{
  UINT32 uwRet;
  TSK_INIT_PARAM_S stTaskInitParam;

  (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
  stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)LOS_BoardKeyTskfunc;
  stTaskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
  stTaskInitParam.pcName = "Key-Demo";
  stTaskInitParam.usTaskPrio = 10;
  uwRet = LOS_TaskCreate(&g_uwKeyTaskID, &stTaskInitParam);

  if (uwRet != LOS_OK)
  {
      return;
  }
  return;  
}

//********************************************************************************
// fn : LOS_BoardKeyTskfunc
//
// brief : 任务入口函数
//
// param : none
//
// return : none
static LITE_OS_SEC_TEXT VOID LOS_BoardKeyTskfunc(VOID)
{
  Button_TypeDef btn = BTN_NONE;
  dispHandle = Display_open(0,NULL);
  
  Display_print0(dispHandle,1,0,4,"LiteOS");
  Display_print0(dispHandle,1,1,0,"Key Test");
  
  while (1)
  {
    btn = LOS_EvbGetKey(0);
    if( btn)
    {
      switch(btn)
      {
        case BTN_UP:
          LOS_EvbUartWriteStr("key up\n");
          LCD_Print("KEY->UP",NULL);
          break;
        case BTN_RIGHT:
          LOS_EvbUartWriteStr("key right\n");
          LCD_Print("KEY->RIGHT",NULL);
          break;
        case BTN_DOWN:
          LOS_EvbUartWriteStr("key down\n");
          LCD_Print("KEY->DOWN",NULL);
          break;
        case BTN_LEFT:
          LOS_EvbUartWriteStr("key left\n");
          LCD_Print("KEY->LEFT",NULL);
          break;
        default:
          break;
      }
    }
    if(btn)
    {
      LOS_EvbLedControl(LOS_LED1, LED_ON);
      (void)LOS_TaskDelay(20);
      LOS_EvbLedControl(LOS_LED1, LED_OFF);
    }
    else
    {
      LOS_TaskDelay(20);
    }

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
