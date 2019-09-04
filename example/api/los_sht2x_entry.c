//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_sht2x_entry.c
//  Date :     2018-11-14 22:50
//  Version :  V0001
//  History :  初始创建版本
//******************************************************************************
#include <string.h>
#include "los_sht2x_entry.h"
#include "los_task.h"

#include "los_bsp_led.h"
#include "los_bsp_key.h"
#include "los_bsp_uart.h"
#include "los_bsp_sht2x.h"

#include "display_cfg.h"

//------------------------------------------------------------------------------
#define LCD_FIRST_LINE       2
#define LCD_LAST_LINE        7

//------------------------------------------------------------------------------
//  Global Variables Declare
static UINT32 g_uwSHT2xTaskID;

//------------------------------------------------------------------------------
// function prototypes

/*  key 任务函数入口 */

static LITE_OS_SEC_TEXT VOID LOS_BoardSHT2xTskfunc(VOID);
/* 显示屏打印信息 */
static void LCD_Print(char *str,char *a);
//------------------------------------------------------------------------------
//******************************************************************************
// fn : LOS_BoardSHT2x_Entry
//
// brief : 创建一个按钮测试任务
//
// param : none
//
// return : none
extern void LOS_BoardSHT2x_Entry(void)
{
  UINT32 uwRet;
  TSK_INIT_PARAM_S stTaskInitParam;

  (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
  stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)LOS_BoardSHT2xTskfunc;
  stTaskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
  stTaskInitParam.pcName = "SHT2X-Demo";
  stTaskInitParam.usTaskPrio = 10;
  uwRet = LOS_TaskCreate(&g_uwSHT2xTaskID, &stTaskInitParam);

  if (uwRet != LOS_OK)
  {
      return;
  }
  return;  
}

//********************************************************************************
// fn : LOS_BoardSHT2xTskfunc
//
// brief : 任务入口函数
//
// param : none
//
// return : none
static LITE_OS_SEC_TEXT VOID LOS_BoardSHT2xTskfunc(VOID)
{
	float temp = 0.0;
  float humi = 0.0;
  dispHandle = Display_open(0,NULL);
  
  Display_print0(dispHandle,1,0,4,"LiteOS");
  Display_print0(dispHandle,1,1,0,"SHT2x Test");
  
  while (1)
  {
    temp = SHT20_Convert(SHT20_ReadTemp(),1);
    humi = SHT20_Convert(SHT20_ReadRH(),0);
		printf("T=%0.1f  H=%0.1f%%\n",temp,humi);
		Display_print2(dispHandle,1,2,0,"T=%0.1f  H=%0.1f%%",temp,humi);
		
		LOS_EvbLedControl(LOS_LED1, LED_ON);
		(void)LOS_TaskDelay(20);
		LOS_EvbLedControl(LOS_LED1, LED_OFF);
		
		LOS_TaskDelay(LOS_MS2Tick(3000));
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
