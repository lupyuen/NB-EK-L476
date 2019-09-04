//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_lux_entry.c
//  Date     : 2018-11-15 15:03
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#include <string.h>
#include "los_lux_entry.h"
#include "los_task.h"

#include "los_bsp_led.h"
#include "los_bsp_key.h"
#include "los_bsp_uart.h"
#include "los_bsp_lux.h"

#include "display_cfg.h"


//------------------------------------------------------------------------------
#define LCD_FIRST_LINE       2
#define LCD_LAST_LINE        7

//------------------------------------------------------------------------------
//  Global Variables Declare
static UINT32 g_uwLUXTaskID;

//------------------------------------------------------------------------------
// function prototypes

/*  key 任务函数入口 */

static LITE_OS_SEC_TEXT VOID LOS_BoardLUXTskfunc(VOID);

/* 显示屏打印信息 */
static void LCD_Print(char *str,char *a);
//******************************************************************************
// fn : LOS_BoardLux_Entry
//
// brief : 创建一个光照度测试任务
//
// param : none
//
// return : none
extern void LOS_BoardLux_Entry(void)
{
  UINT32 uwRet;
  TSK_INIT_PARAM_S stTaskInitParam;

  (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
  stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)LOS_BoardLUXTskfunc;
  stTaskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
  stTaskInitParam.pcName = "Bright-Demo";
  stTaskInitParam.usTaskPrio = 10;
  uwRet = LOS_TaskCreate(&g_uwLUXTaskID, &stTaskInitParam);

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
static LITE_OS_SEC_TEXT VOID LOS_BoardLUXTskfunc(VOID)
{
  uint16_t ad;
  uint16_t lux;
  dispHandle = Display_open(0,NULL);
  
  Display_print0(dispHandle,1,0,4,"LiteOS");
  Display_print0(dispHandle,1,1,0,"Bright Test");
  
  while (1)
  {
    ad = LOS_EvbLux_Read();
    lux = LOS_EvbLux_Convert(ad);
		printf("ADC=%04X  LUX=%d\n",ad,lux);
    
		Display_print1(dispHandle,1,2,0,"ADC=%04X",ad);
    Display_print1(dispHandle,1,3,0,"LUX=%d",lux);
		
		LOS_EvbLedControl(LOS_LED1, LED_ON);
		(void)LOS_TaskDelay(20);
		LOS_EvbLedControl(LOS_LED1, LED_OFF);
		
		LOS_TaskDelay(LOS_MS2Tick(1000));
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
