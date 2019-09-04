//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_light_entry.c
//  Date     : 2018-11-20 17:22
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#include <string.h>
#include "los_gps_entry.h"
#include "los_task.h"

#include "los_bsp_uart.h"
#include "los_bsp_led.h"
#include "los_bsp_key.h"

#include "los_bsp_gps.h"
#include "gps_l70_r.h"

#include "display_cfg.h"

//------------------------------------------------------------------------------
#define LCD_FIRST_LINE       2
#define LCD_LAST_LINE        7

#define GPS_NMEA_RMC        "NMEA-RMC"
#define GPS_NMEA_ALL        "NMEA-ALL"
#define GPS_HOT_START       "HOT START"


//******************************************************************************
//   定义GPS事件类型
//    
#define GPS_EVENT_GPRMC             0x01
    
static uint8_t gps_event_table = 0;
//------------------------------------------------------------------------------
//  Global Variables Declare
static UINT32 g_uwGpsTaskID;

//------------------------------------------------------------------------------
// function prototypes

/*  key 任务函数入口 */

static LITE_OS_SEC_TEXT VOID LOS_BoardGpsTskfunc(VOID);

/* 显示gps 数据*/
static void  Display_Gps(void);
/* 显示屏打印信息 */
static void LCD_Print(char *str,char *a);
//******************************************************************************
// fn : LOS_BoardGPS_Entry
//
// brief : 创建一个GPS测试任务
//
// param : none
//
// return : none
extern void LOS_BoardGPS_Entry(void)
{
  UINT32 uwRet;
  TSK_INIT_PARAM_S stTaskInitParam;

  (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
  stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)LOS_BoardGpsTskfunc;
  stTaskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
  stTaskInitParam.pcName = "GPS-Demo";
  stTaskInitParam.usTaskPrio = 10;
  uwRet = LOS_TaskCreate(&g_uwGpsTaskID, &stTaskInitParam);

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
static LITE_OS_SEC_TEXT VOID LOS_BoardGpsTskfunc(VOID)
{
  uint8_t upCnt = 0;
  char*   info = NULL;
	Button_TypeDef btn = BTN_NONE;
  GPS_Init();
  dispHandle = Display_open(0,NULL);
#ifdef UART_DBG
  printf("LiteOS GPS Example\n");
#endif
  Display_print0(dispHandle,1,0,4,"LiteOS");
  Display_print0(dispHandle,1,1,0,"GPS Search...");
  
  while (1)
  {
    //轮询串口收发业务
    LOS_EvbGpsUartPoll();
    
    if(gps_event_table)
    {
      if(gps_event_table & GPS_EVENT_GPRMC)
      {
        Display_Gps();
        gps_event_table ^= GPS_EVENT_GPRMC;
      }
    }
    
    btn = LOS_EvbGetKey(0);
    if( btn)
    {
      switch(btn)
      {
        case BTN_UP:
          upCnt++;
          if(upCnt % 2)
          {
            //只输出GPSRMC信息
            GPS_SetNMEARMC();
            info = GPS_NMEA_RMC;
            printf("NMEA-RMC\n");
          }
          else 
          {
            //恢复输出
            GPS_SetNMEADefault();
            info = GPS_NMEA_ALL;
            printf("NMEA-ALL\n");          
          }
          break;
        case BTN_RIGHT:
          info = "Unknown";
          break;
        case BTN_DOWN:
          
          GPS_HotStart();
          info = GPS_HOT_START;
          printf("Hot Start\n");   
          break;
        case BTN_LEFT:
          info = "Unknown";
          break;
        default:
          break;
      }
    }
		if(btn)
    {
      LOS_EvbLedControl(LOS_LED1, LED_ON);
      Display_print1(dispHandle,1,7,0,"%s",info);
      LOS_EvbLedControl(LOS_LED1, LED_OFF);
    }
    LOS_TaskDelay(1);
  }
}
//*****************************************************************************
// fn : AppGpsMsg_cb
//
// brief : gps callback
//
// param : type -> the type of msg
//
// return : none
void AppGpsMsg_cb(gps_type_e type)
{
  
  switch(type)
  {
  case GPS_TYPE_RMC:
    {
      gps_event_table |= GPS_EVENT_GPRMC;
    }
    break;
  }
}
//******************************************************************************
// fn : Display_Gps
//
// brief : 在显示屏与串口打印定位信息
//
// param : none 
//
// return : none
void  Display_Gps(void)
{
  //获取gps信息地址
  GpsrmcHandle gps =  GPS_Get();
 
  if(gps)
  {
    Display_print0(dispHandle,1,1,0,"GPS OK");
    Display_print0(dispHandle,1,2,0,gps->utc);
    Display_print2(dispHandle,1,3,0,"%s:%s",gps->latitude_value,gps->latitude);
    Display_print2(dispHandle,1,4,0,"%s:%s",gps->longitude_value,gps->longitude);
    Display_print1(dispHandle,1,5,0,"V=%s knots",gps->speed);
    Display_print0(dispHandle,1,6,0,gps->date);
#ifdef UART_DBG
    printf("utc=%s\r\n",gps->utc);
    printf("Latitude=%s:%s\r\n",gps->latitude_value,gps->latitude);
    printf("Longitude=%s:%s\r\n",gps->longitude_value,gps->longitude);
    printf("Speed=%s knots\r\n",gps->speed);
    printf("Date=%s\r\n",gps->date);
#endif    

  }
}
