//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_light_entry.c
//  Date     : 2018-11-14 23:06
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#include <string.h>
#include "los_light_entry.h"
#include "los_task.h"

#include "los_bsp_led.h"
#include "los_bsp_key.h"
#include "los_bsp_uart.h"
#include "los_bsp_pwm.h"

#include "display_cfg.h"


//------------------------------------------------------------------------------
#define LCD_FIRST_LINE       2
#define LCD_LAST_LINE        7
//------------------------------------------------------------------------------
//  Global Variables Declare
static UINT32 g_uwLightTaskID;

//------------------------------------------------------------------------------
// function prototypes

/*  key 任务函数入口 */

static LITE_OS_SEC_TEXT VOID LOS_BoardLightTskfunc(VOID);

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
extern void LOS_BoardLight_Entry(void)
{
  UINT32 uwRet;
  TSK_INIT_PARAM_S stTaskInitParam;

  (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
  stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)LOS_BoardLightTskfunc;
  stTaskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
  stTaskInitParam.pcName = "PWM-Demo";
  stTaskInitParam.usTaskPrio = 10;
  uwRet = LOS_TaskCreate(&g_uwLightTaskID, &stTaskInitParam);

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
static LITE_OS_SEC_TEXT VOID LOS_BoardLightTskfunc(VOID)
{
	Button_TypeDef btn = BTN_NONE;
  uint8_t duty = 0;
	uint8_t onFlag = 0;
  dispHandle = Display_open(0,NULL);
  
  Display_print0(dispHandle,1,0,4,"LiteOS");
  Display_print0(dispHandle,1,1,0,"Light Test");
	Display_print0(dispHandle,1,2,0,"OFF <-> UP");
	Display_print0(dispHandle,1,3,0,"Duty=0");
  
  while (1)
  {
    btn = LOS_EvbGetKey(0);
    if( btn)
    {
      switch(btn)
      {
        case BTN_UP:
					onFlag++;
					if(onFlag % 2)
					{
						Display_print0(dispHandle,1,2,0,"ON <-> UP");
						duty = 50;
						Display_print1(dispHandle,1,3,0,"Duty=%d",duty);
						LOS_EvbPwm_Set(duty);
					}
					else
					{
						Display_print0(dispHandle,1,2,0,"OFF <-> UP");
						Display_print0(dispHandle,1,3,0,"Duty=0");
						LOS_EvbPwm_Set(0);
					}
          break;
        case BTN_RIGHT:
          if(onFlag % 2)
					{
						if(duty < 100)
						{
							duty++;
							Display_print1(dispHandle,1,3,0,"Duty=%d",duty);
							LOS_EvbPwm_Set(duty);
						}
							
					}
          break;
        case BTN_DOWN:

          break;
        case BTN_LEFT:
          if(onFlag % 2)
					{
						if(duty > 0)
						{
							duty--;
							Display_print1(dispHandle,1,3,0,"Duty=%d",duty);
							LOS_EvbPwm_Set(duty);
						}
					}
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
