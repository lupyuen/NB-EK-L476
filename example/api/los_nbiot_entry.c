//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_nbiot_entry.c
//  Date     : 2018-11-13 22:43
//  Version  : V0001
// ��ʷ��¼  : ��һ�δ���
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
  NB_INIT,          // ��ʼ������
  NB_MODULE,        // ��ȡNBģ�鳧�̼��̼���Ƶ�ε���Ϣ
  NB_SIGN,
  NB_UDP_CR,        // ����UDP
  NB_UDP_CL,        // �ر�UDP
  NB_UDP_REG,       // ��״̬����ע�������ƽ̨��Ϣ��������������ƽ̨ͨ�ţ��ɺ��ԣ�
  NB_UDP_ST,        // �����Ѿ�������UDP��������
  NB_UDP_RE,        // UDP������Ϣ
  NB_CoAP_SEVER,    // CoAPԶ�̵�ַ�������ȡ
  NB_CoAP_ST,       // ����CoAP������Ϣ
  NB_CoAP_RE,       // CoAP������Ϣ
  NB_RESET,         // ��λNB
  NB_END
}NB_STATE_e;


//��¼NBģ���ʼ������˳��
#define  NB_SEQ_INIT         0x01
#define  NB_SEQ_UDP_CR       0X02
#define  NB_SEQ_UDP_REG      0x04
#define  NB_SEQ_COAP_SERVER  0x08

//LCD ��ʼ�������ʾ���±�
#define LCD_FIRST_LINE       2
#define LCD_LAST_LINE        7

//NB����״̬LED��˸�¼�
#define NB_LED_NONET_ID      0x0001    //Ĭ��״̬
#define NB_LED_ONNET_ID      0x0002    //��������

#define NB_LED_MASK_ID       0x0003    //�¼�����

//------------------------------------------------------------------------------
// Global Variable declare
//
static UINT32        g_uwNBiotTaskID;      //NB������
static UINT32        g_uwLedTaskID;        //Led��ʾ������
static EVENT_CB_S    net_state_event;      //NB����״̬�¼��ṹ

static volatile NB_STATE_e  APP_STATE= NB_NONE;   //NB����״̬����
static uint8_t              seq_record = 0;       //��־�ǼǱ���
static uint8_t*             NB_Module_IMEI = NULL;//��¼ģ���IMEI��

//------------------------------------------------------------------------------
// Function prototypes declare
//
/* NBģ����Ϣ�ϱ��ص� */
#ifdef LOS_NBIOT_EN
//�˴��ŵ�Ԥ�����У���Ҫ��Ϊ�˷�ֹ��ζ������
int NB_MsgreportCb(msg_types_t,int ,char*);
#endif

/* ��ʾ����ӡ��Ϣ */
static void LCD_Print(char *str,char *a);

/* LED ��������� */
static LITE_OS_SEC_TEXT VOID LOS_LED_Tskfunc(VOID);
//******************************************************************************
// fn : LOS_NbIot_Tskfunc
//
// brief : NBIOT ������
//
// param : none
//
// return : none
static LITE_OS_SEC_TEXT VOID LOS_NbIot_Tskfunc(VOID)
{
  Color fontColor = GREEN;
  seq_record = 0; 
  
  //����ʾ��
  dispHandle = Display_open(0,NULL);
  
  //������ʾ��ǰ��ɫ
  Display_control(dispHandle,CMD_FRONT_COLOR,&fontColor);
  
  //��ʾ������Ϣ
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
        //����ע���
        //������������ƽ̨ͨ�ţ�����Ҫ����ע���
        printf("\r\n<----NB registes to Cloud ---->\r\n");
        LCD_Print("Udp send...",NULL);
        
        char regPacket[30];
        uint8_t msgLen = 0;
        
        //���ÿ⺯�����кϸ�����
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
// brief : LED ������
//
// param : none
//
// return : none
static LITE_OS_SEC_TEXT VOID LOS_LED_Tskfunc(VOID)
{
  UINT32   uwEvent = 0;
  UINT32   tmp = 0;   
  //led ��˸��Ϣ�ṹ
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
    
    //����LED��ʱ��
    tmp = (uint32)(ledState[led_index].period * ((double)ledState[led_index].duty / 100.0));
    LOS_TaskDelay(tmp);
    
    LOS_EvbLedControl(LOS_LED1, LED_OFF);
    
    //�������ʱ��
    tmp = ledState[led_index].period - tmp;
    LOS_TaskDelay(tmp);
  }
}
//******************************************************************************
// fn : LOS_NBIOT_Entry
//
// brief : ����nbiotϵͳ����
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
// brief : NBģ����Ϣ�ϱ��ص�
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
        seq_record |= NB_SEQ_INIT;  //��¼��ʼ���ɹ�
        
        //��LED״̬
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
        APP_STATE = NB_CoAP_SEVER;       //�趨Զ�̵�ַ
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
      //����IMEI��Ϣ
      NB_Module_IMEI = (uint8_t*)msg;
    }
    break;
  case MSG_UDP_CREATE:
    {
      printf("\r\nUDP_CR=%s\r\n",msg);
      LCD_Print("UDP=%s",msg);
      if(*msg == 'S')  //"S" ��ʾ�����ɹ���'F'��ʾʧ��
      {
        seq_record |= NB_SEQ_UDP_CR;  //��¼��ʼ���ɹ�
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
        //���UDP �����ɹ���־
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
        //�������
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
// brief : ������ӡ��Ϣ
//
// param : str -> �ַ�����ʽ
//         a   -> ����
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
