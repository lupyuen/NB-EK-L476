//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_nbiot_entry.c
//  Date     : 2018-11-13 22:43
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#include <stdio.h>
#include <string.h>

#include "los_bsp_nbuart.h"
#include "los_bsp_led.h"
#include "los_nbiot_entry.h"
#include "los_task.h"
#include "los_event.h"

#include "NB_Board.h"
#include "NB_BC95.h"
#include "NB_Board_Cfg.h"

#include "Display.h"
#include "DisplayTFT128.h"
#include "TFT128.h"
#include "Display_cfg.h"

//------------------------------------------------------------------------------
// Typedef 
//
typedef enum
{
  NB_NONE,
  NB_INIT,          // 初始化操作
  NB_MODULE,        // 获取NB模块厂商及固件，频段等信息
  NB_SIGN,
  NB_UDP_CR,        // 创建UDP
  NB_UDP_CL,        // 关闭UDP
  NB_UDP_REG,       // 此状态用于注册谷雨云平台信息（如果不与谷雨云平台通信，可忽略）
  NB_UDP_ST,        // 利用已经创建的UDP发送数据
  NB_UDP_RE,        // UDP接收信息
  NB_CoAP_SEVER,    // CoAP远程地址设置与获取
  NB_CoAP_ST,       // 利用CoAP发送消息
  NB_CoAP_RE,       // CoAP返回信息
  NB_RESET,         // 复位NB
  NB_END
}NB_STATE_e;


//记录NB模块初始化流程顺序
#define  NB_SEQ_INIT         0x01
#define  NB_SEQ_UDP_CR       0X02
#define  NB_SEQ_UDP_REG      0x04
#define  NB_SEQ_COAP_SERVER  0x08

//LCD 启始与结束显示行下标
#define LCD_FIRST_LINE       2
#define LCD_LAST_LINE        7

//NB网络状态LED闪烁事件
#define NB_LED_NONET_ID      0x0001    //默认状态
#define NB_LED_ONNET_ID      0x0002    //附着网络

#define NB_LED_MASK_ID       0x0003    //事件掩码

//------------------------------------------------------------------------------
// Global Variable declare
//
static UINT32        g_uwNBiotTaskID;      //NB任务句柄
static UINT32        g_uwLedTaskID;        //Led显示任务句柄
static EVENT_CB_S    net_state_event;      //NB网络状态事件结构

static volatile NB_STATE_e  APP_STATE= NB_NONE;   //NB任务状态变量
static uint8_t              seq_record = 0;       //标志登记变量
static uint8_t*             NB_Module_IMEI = NULL;//记录模块的IMEI号

//------------------------------------------------------------------------------
// Function prototypes declare
//
/* NB模块消息上报回调 */
#ifdef LOS_NBIOT_EN
//此处放到预处理中，主要是为了防止多次定义错误
int NB_MsgreportCb(msg_types_t,int ,char*);
#endif

/* 显示屏打印信息 */
static void LCD_Print(char *str,char *a);

/* LED 任务函数入口 */
static LITE_OS_SEC_TEXT VOID LOS_LED_Tskfunc(VOID);
//******************************************************************************
// fn : LOS_NbIot_Tskfunc
//
// brief : NBIOT 任务函数
//
// param : none
//
// return : none
static LITE_OS_SEC_TEXT VOID LOS_NbIot_Tskfunc(VOID)
{
  Color fontColor = GREEN;
  seq_record = 0; 
  
  //打开显示屏
  dispHandle = Display_open(0,NULL);
  
  //设置显示的前景色
  Display_control(dispHandle,CMD_FRONT_COLOR,&fontColor);
  
  //显示测试信息
  Display_print0(dispHandle,1,0,4,"NB-IOT");
  Display_print0(dispHandle,1,1,0,"Net:OFF ---dbm");
  
  NBModule_open(&nb_config);
  APP_STATE = NB_NONE;
  
  for(;;)
  {
    LOS_EvbNBUartPoll();
    NBModule_Main(&nb_config);
		switch(APP_STATE)
    {
    case NB_NONE:
      {
        //wait for key
        APP_STATE = NB_INIT;
      }
      break;
    case NB_INIT:
      {
        printf("\r\n<----BC95 Init---->\r\n");
        LCD_Print("Init start...",NULL);
        seq_record = 0;
        NBModule_Init(&nb_config);
        APP_STATE = NB_END;
      }
      break;
    case NB_SIGN:
      {
         printf("\r\n<----BC95 get signal---->\r\n");
         NBModule_Sign(&nb_config);
         APP_STATE = NB_END;
      }
      break;
    case NB_MODULE:
      {
        printf("\r\n<----Module info ---->\r\n");
        LCD_Print("Module info...",NULL);
        NBModule_Info(&nb_config);
        APP_STATE = NB_END;
      }
      break;
    case NB_UDP_CR:
      {
        printf("\r\n<----Create udp ---->\r\n");
        LCD_Print("UDP Create...",NULL);
        NBModule_CreateUDP(&nb_config);
        APP_STATE = NB_END;
      }
      break;
    case NB_UDP_CL:
      {
        printf("\r\n<----Close udp ---->\r\n");
        NBModule_CloseUDP(&nb_config);
        APP_STATE = NB_END;
      }
      break;
    case NB_UDP_REG:
      {
        //发送注册包
        //如果不与谷雨云平台通信，则不需要发送注册包
        printf("\r\n<----NB registes to Cloud ---->\r\n");
        LCD_Print("Udp send...",NULL);
        
        char regPacket[30];
        uint8_t msgLen = 0;
        
        //利用库函数进行合格化数据
        msgLen = sprintf(regPacket,"ep=%s&pw=123456",NB_Module_IMEI);
        regPacket[msgLen] = 0;
        
        //char* regPacket = "ep=863703036005069&pw=123456";
        NBModule_SendData(&nb_config,msgLen,regPacket);
        seq_record |= NB_SEQ_UDP_REG;
        LCD_Print("S3 send UDP",NULL);
        APP_STATE = NB_END;
      }
      break;
    case NB_UDP_ST:
      {
        printf("\r\n<---- Send udp ---->\r\n");
        LCD_Print("Udp send...",NULL);
       	char* userPacket = "NB-EK-L476";
        NBModule_SendData(&nb_config,sizeof("NB_EK_L476"),userPacket);

        APP_STATE = NB_END;
      }
      break;
    case NB_CoAP_SEVER:
      {
        printf("\r\n<---- CoAP Server set ---->\r\n");
        LCD_Print("Coap remote...",NULL);
        
        NBModule_CoAPServer(&nb_config,1,NULL);
        APP_STATE = NB_END;
      }
      break;
    case NB_CoAP_ST:
      {
        printf("\r\n<---- CoAP Send ---->\r\n");
        char test_str[] = "NB-EN-L476";
        bc95_coapSendMsg(&nb_config,sizeof(test_str) -1 ,test_str);

        APP_STATE = NB_END;
      }
      break;
    default:
      {
      }
      break;
    }
    LOS_TaskDelay(1);
  }
}
//******************************************************************************
// fn : LOS_LED_Tskfunc
//
// brief : LED 任务函数
//
// param : none
//
// return : none
static LITE_OS_SEC_TEXT VOID LOS_LED_Tskfunc(VOID)
{
  UINT32   uwEvent = 0;
  UINT32   tmp = 0;   
  //led 闪烁信息结构
  typedef struct
  {
    uint16_t period;
    uint8_t  duty;
  }led_state_t;
  
  led_state_t ledState[] = {{1000,50},{1000,10}};
  uint8_t  led_index = 0;
  for(;;)
  {
    uwEvent = LOS_EventRead(&net_state_event, NB_LED_MASK_ID, LOS_WAITMODE_OR|LOS_WAITMODE_CLR,0);
    if(uwEvent)
    {
      if(uwEvent & NB_LED_NONET_ID)
      {
        led_index = 0;
      }
      if(uwEvent & NB_LED_ONNET_ID)
      {
        led_index = 1;
      }
    }
    LOS_EvbLedControl(LOS_LED1, LED_ON);
    
    //计算LED亮时间
    tmp = (uint32)(ledState[led_index].period * ((double)ledState[led_index].duty / 100.0));
    LOS_TaskDelay(tmp);
    
    LOS_EvbLedControl(LOS_LED1, LED_OFF);
    
    //计算灭的时间
    tmp = ledState[led_index].period - tmp;
    LOS_TaskDelay(tmp);
  }
}
//******************************************************************************
// fn : LOS_NBIOT_Entry
//
// brief : 创建nbiot系统任务
//
// param : none
//
// return : none
void LOS_NBIOT_Entry(void)
{
    UINT32 uwRet;
    TSK_INIT_PARAM_S stTaskInitParam;

    (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)LOS_NbIot_Tskfunc;
    stTaskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;
    stTaskInitParam.pcName = "nb-iot";
    stTaskInitParam.usTaskPrio = 5;
    uwRet = LOS_TaskCreate(&g_uwNBiotTaskID, &stTaskInitParam);

    if (uwRet != LOS_OK)
    {
#ifdef UART_DBG
      printf("NB-IOT task create failed\n");
#endif
      return;
    }

    (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    stTaskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)LOS_LED_Tskfunc;
    stTaskInitParam.uwStackSize  = 0x200;
    stTaskInitParam.pcName       = "LED";
    stTaskInitParam.usTaskPrio   = LOS_TASK_PRIORITY_LOWEST -1;
    uwRet = LOS_TaskCreate(&g_uwLedTaskID, &stTaskInitParam);
    
    if (uwRet != LOS_OK)
    {
#ifdef UART_DBG
      printf("LED task create failed\n");
#endif
      return;
    }
    
    uwRet = LOS_EventInit(&net_state_event);
    if (uwRet != LOS_OK)
    {
#ifdef UART_DBG
      printf("NET event create failed\n");
#endif
      return;
    }
}

#ifdef LOS_NBIOT_EN
//******************************************************************************
// fn : NB_MsgreportCb
//
// brief : NB模块消息上报回调
//
// param : 
//
// return : none
int  NB_MsgreportCb(msg_types_t types,int len,char* msg)
{
  switch(types)
  {
  case MSG_INIT:
    {
      printf("\r\nINIT=%s\r\n",msg);
      LCD_Print("Init=%s",msg);
      if(*msg == 'S')
      {
        Display_print0(dispHandle,0,1,0,"NET=ON");
        APP_STATE = NB_SIGN;
        seq_record |= NB_SEQ_INIT;  //记录初始化成功
        
        //置LED状态
        LOS_EventWrite(&net_state_event, NB_LED_ONNET_ID);
      }
      else
      {
        printf("Reinit NB with S1\r\n");
        LCD_Print("Reinit NB...",NULL);
      }
    }
    break;
  case MSG_IMSI:
    {
      printf("\r\nIMSI=%s\r\n",msg);
      LCD_Print(msg,NULL);
    }
    break;
  case MSG_REG:
    {
      Display_print1(dispHandle,0,1,0,"NET=%s",(*msg) == 1 ?"ON":"0FF");
    }
    break;
  case MSG_SIGN:
    {
      if('F' != (*msg))
      {
        Display_print1(dispHandle,1,1,8,"%sdbm",msg);
        APP_STATE = NB_CoAP_SEVER;       //设定远程地址
      }
    }
    break;
  case MSG_MODULE_INFO:
    {
      printf("\r\nMinfo=%s\r\n",msg);
    }
    break;
  case MSG_MID:
    {
      printf("\r\nMID=%s\r\n",msg);
    }
    break;
  case MSG_MMODEL:
    {
      printf("\r\nModel=%s\r\n",msg);
    }
    break;
  case MSG_MREV:
    {
      printf("\r\nREV=%s\r\n",msg);
    }
    break;
  case MSG_BAND:
    {
      printf("\r\nFreq=%s\r\n",msg);
    }
    break;
  case MSG_IMEI:
    {
      printf("\r\nIMEI=%s\r\n",msg);
      //保存IMEI信息
      NB_Module_IMEI = (uint8_t*)msg;
    }
    break;
  case MSG_UDP_CREATE:
    {
      printf("\r\nUDP_CR=%s\r\n",msg);
      LCD_Print("UDP=%s",msg);
      if(*msg == 'S')  //"S" 表示创建成功，'F'表示失败
      {
        seq_record |= NB_SEQ_UDP_CR;  //记录初始化成功
      }
      else
      {
        printf("Please, recreate udp\r\n");
      }
    }
    break;
  case MSG_UDP_CLOSE:
    {
      printf("\r\nUDP_CL=%s\r\n",msg);

      if(*msg == 'S')
      {
        //清除UDP 创建成功标志
        seq_record &= ~NB_SEQ_UDP_CR;
      }
    }
    break;
  case MSG_UDP_SEND:
    {
      printf("\r\nUDP_SEND=%s\r\n",msg);
      LCD_Print("Send=%s",msg);
    }
    break;
  case MSG_UDP_RECE:
    {
      printf("\r\nUDP_RECE=%s\r\n",msg);
      LCD_Print(msg,NULL);
    }
    break;
  case MSG_COAP:
    {
      printf("\r\nCOAP=%s\r\n",msg);
      LCD_Print("COAP=%s",msg);
      if(*msg == 'S')
      {
        //针对设置
        seq_record |= NB_SEQ_COAP_SERVER;
      }
    }
    break;
  case MSG_COAP_SEND:
    {
      printf("\r\nCOAP_SENT=%s\r\n",msg);
    }
    break;
    
  case MSG_COAP_RECE:
    {
      printf("\r\nCOAP_RECE=%s\r\n",msg);
      LCD_Print("Rece=%s",msg);
    }
    break;
  default :
    {
      break;
    }
  }
  return 0;
}
#endif
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
