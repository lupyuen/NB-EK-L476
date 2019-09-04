//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : NB_BC95.h
//  Date     : 2017-12-04 16:11
//  Version  : V0001
// ��ʷ��¼  : ��һ�δ���
//******************************************************************************

#ifndef NB_BC95_H
#define NB_BC95_H

#ifdef __cplusplus
extern "C"
{
#endif
 
#include <stdint.h>
#include "NB_Board.h"
  
//******************************************************************************
//BC95ϵ��ģ�飬��Ҫ�ǻ��� UART���û����н������������й���BC95�Ĳ�������Χ��
//UART��
//
  
  
/*
 * cmd ����ö��
 */
typedef enum
{
	CMD_TEST,         //����TEST����
	CMD_READ,         //����READ����
	CMD_SET,          //����SET����
	CMD_EXCUTE        //����EXCUTE����
}cmd_property_t;

/*
 * ATָ�����Ϊö��
 */
typedef enum
{
  ACTION_OK_EXIT_ERROR_NEXT,     //�ɹ�ִ�к��˳������󽫼���ִ����һ��ָ��
  ACTION_OK_NEXT_ERROR_TRY       //�ɹ�ִ�к����ִ����һ��ָ��������г���
                                 //������ﵽ����Դ�������û�гɹ������˳�
}cmd_action_t;

//ATָ��ṹ����
typedef struct
{
	const char*     pCMD;          //  ATָ��
	cmd_property_t  property;      // ָ�ǰ����(TEST,READ,SET,EXCUTE)
	char*           pArgument;     // ָ�����
  char*           pExpectRes;    // �����õ��ظ�
	unsigned char   cmd_try;       // �����Դ���
  unsigned char   haveTried;     // �Ѿ����γ��ԵĴ���
  cmd_action_t    cmd_action;    // ATָ����Ϊ
  uint16_t        max_timeout;   // ���ʱʱ��
  uint8_t         lmt_period;    // �ظ�ִ���޶�ʱ����
}cmd_info_t;

//����cmd�ṹָ������
typedef cmd_info_t* CmdHandle;

//******************************************************************************
// Name: process_e
//
// brief :��ʾ��ǰҪ��bc95���еĲ���
typedef enum
{
  PROCESS_NONE,     //��ʾbc95ģ�鵱ǰ����Ϊ���У����Խ�����һ��Ϊ����
  PROCESS_INIT = MSG_INIT,
  PROCESS_MODULE_INFO = MSG_MODULE_INFO,
  PROCESS_SIGN = MSG_SIGN,
  PROCESS_NET_REG = MSG_REG,
  PROCESS_UDP_CR = MSG_UDP_CREATE,
  PROCESS_UDP_CL = MSG_UDP_CLOSE,
  PROCESS_UDP_ST = MSG_UDP_SEND,
  PROCESS_UDP_RE = MSG_UDP_RECE,
  PROCESS_COAP  = MSG_COAP,
  PROCESS_COAP_ST = MSG_COAP_SEND,
  PROCESS_COAP_RE = MSG_COAP_RECE
}process_t;

typedef enum
{
  TYPES_CIMI = MSG_IMSI,
  TYPES_CGSN = MSG_IMEI,
  TYPES_CGMI = MSG_MID,
  TYPES_CGMM = MSG_MMODEL,
  TYPES_CGMR = MSG_MREV,
  TYPES_NBAND = MSG_BAND,
  TYPES_UDP_CR = MSG_UDP_CREATE,
  TYPES_UDP_CL = MSG_UDP_CLOSE,
  TYPES_UDP_SEND = MSG_UDP_SEND,
  TYPES_UDP_RECE = MSG_UDP_RECE
}report_msgType_e;

typedef struct
{
  process_t    state;
  int          sub_state;
}module_state;


//��������UART���ջص�
typedef void (*bc95_receive_cb)(char*, uint16_t);
//==============================================================================

/*
 * ���崮�ڲ�����غ���ָ��
 */
typedef void (*com_open)(bc95_receive_cb, uint32_t);
typedef void (*com_send)(uint8_t*,uint16_t);
typedef void (*com_close)(void);

typedef struct
{
  com_open  openFxn;
  com_send  sendFxn;
  com_close closeFxn;
}com_fxnTable;

typedef void (*bc95_timeout_cb)(void);

//==============================================================================
typedef void (*timer_init)(bc95_timeout_cb);
typedef void (*timer_start)(uint32_t);
typedef void (*timer_stop)(void);

typedef struct
{
  timer_init   initTimerFxn;
  timer_start  startTimerFxn;
  timer_stop   stopTimerFxn;
}time_fxnTable;


//bc95����ṹ˵��
typedef struct
{
  const uint32         baudrate;
  const com_fxnTable   *uart_fxnTable;
  
  const time_fxnTable  *timer_fxnTable;
}hw_object_t;

typedef hw_object_t* HWAttrs_Handle;

//==============================================================================

/*
 * ��ز�����������
 */
   
//******************************************************************************
// fn : bc95_open
//
// brief : ��bc95ʹ�õ�uart����Ӳ��
//
// param : handle ->nb����ָ��
//
// return : none
extern int bc95_open(NB_Handle handle);

//******************************************************************************
// fn : bc95_init
//
// brief : ��ʼ��BC95ģ�飬�����������ʼ��
//
// param : handle ->nb����ָ��
//
// return : none
extern int bc95_init(NB_Handle handle);

//******************************************************************************
// fn : bc95_moduleInfo
//
// brief : ��ȡNBģ�����Ϣ�����磺Manufacturer ID, Device Module,
//         Firmware Version,Freqenucy band
//
// param : handle ->nb����ָ��
//
// return : none
extern int bc95_moduleInfo(NB_Handle handle);

//******************************************************************************
// fn : bc95_register
//
// brief : ��ѯNBģ���Ƿ�ע������
//
// param : handle ->nb����ָ��
//
// return : none
extern int bc95_register(NB_Handle handle);

//******************************************************************************
// fn : bc95_getIMSI
//
// brief : ��ȡģ��IMSI��
//
// param : handle ->nb����ָ��
//
// return : none
extern const char* bc95_getIMSI(NB_Handle handle);

//******************************************************************************
// fn : bc95_getSignal
//
// brief : ��ȡ��ǰ�ź�ǿ��
//
// param : handle ->nb����ָ��
//
// return : none
extern int bc95_getSignal(NB_Handle handle);
//******************************************************************************
// fn : bc95_createUDP
//
// brief : ����UDP
//
// param : handle ->nb����ָ��
//
// return : none
extern int bc95_createUDP(NB_Handle handle);
//******************************************************************************
// fn : bc95_closeUDP
//
// brief : �رյ�ǰ����UDP
//
// param : handle ->nb����ָ��
//
// return : none
extern int bc95_closeUDP(NB_Handle handle);

//******************************************************************************
// fn : bc95_sendUDP
//
// brief : ����UDP����
//
// param : handle -> NB �ṹ��Ϣָ��
//         len -> ��Ϣ����
//         msg -> ��Ϣ
//
// return : none
extern int bc95_sendUDP(NB_Handle handle,int len,char* msg);
//******************************************************************************
// fn : bc95_coapServer
//
// brief : ��ѯ������bc95COAP�������Ϣ
//
// param : handle -> NB �ṹ��Ϣָ��
//         isSet  -> true -> write,
//                   false-> read
//
// return : sucess or fail
extern int bc95_coapServer(NB_Handle handle,Bool isSet,char* coap);

//******************************************************************************
// fn : bc95_coapSentIndication
//
// brief : ����CoAP������Ϣ����ָʾ��ʽ
//
// param : handle -> NB �ṹ��Ϣָ��
//         code   -> 0  �����κη�Ӧ
//                   !0 ��Ӧ��Ϣ
//
// return : none
extern int bc95_coapSentIndication(NB_Handle handle,int code);

//******************************************************************************
// fn : bc95_coapReceIndication
//
// brief : ����CoAP������Ϣ��ʾ��ʽ
//
// param : handle -> NB �ṹ��Ϣָ��
//         code   -> 0  code -> 0��ʾ���棬1 ��ʾֱ�ӽ��ա���Ŀǰ��֧������ģʽ��
//
// return : none
extern int bc95_coapReceIndication(NB_Handle handle,int code);

//******************************************************************************
// fn : bc95_coapSendMsg
//
// brief : ����COAP������Ϣ��COAP������
//
// param : handle -> NB �ṹ��Ϣָ��
//         len    -> ��Ϣ����
//         msg    -> ��Ϣ����
//
// return : none
extern int bc95_coapSendMsg(NB_Handle handle,int len,char*msg);

//******************************************************************************
// fn : bc95_main
//
// brief : ����BC95��������
//
// param : none
//
// return : FALSE -> ����ʧ�ܣ�TRUE -> �����ɹ�
extern int bc95_main(NB_Handle handle);
#ifdef __cplusplus
}
#endif
#endif   //NB_BC95_H
