//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : nb_Board.h
//  Date     : 2017-12-11 11:18
//  Version  : V0001
// ��ʷ��¼  : ��һ�δ���
//******************************************************************************
#ifndef NB_BOARD_H
#define NB_BOARD_H

#ifdef __cplusplus
extern "C"
{
#endif
  
#include "nb_types.h" 
//******************************************************************************
// Const define 
//******************************************************************************
  
//Error const number
 
#define SUCCESS     0

#define FAIL        1
  

//******************************************************************************
//Typedef 
//******************************************************************************

//���������Ϣ����
typedef enum
{
  MSG_NONE,
  //================
  MSG_INIT,
  MSG_IMSI,
  //================
  MSG_MODULE_INFO,
  MSG_MID,     //������ID
  MSG_MMODEL,  //�����ͺ�
  MSG_MREV,    //���Ұ汾��
  MSG_BAND,    //����Ƶ��
  MSG_IMEI,    //�ƶ��豸�����
  //===================================
  MSG_SIGN,    //�ź�ǿ��
  MSG_REG,
  //===================================
  MSG_UDP_CREATE,
  MSG_UDP_CLOSE,
  MSG_UDP_SEND,
  MSG_UDP_RECE,
  //
  MSG_COAP,
  MSG_COAP_SEND,
  MSG_COAP_RECE,
  
  MSG_END
}msg_types_t;

//ָ��ָ����NBiotģ��
typedef struct NB_Config *NB_Handle;


//��NBģ���������ǰ�ڵĳ�ʼ������
typedef int (*NB_Open)(NB_Handle);

//��NBģ�������ռ�õ���Դ�����ͷ�
typedef int (*NB_Close)(NB_Handle);

//�Զ�������س�ʼ��ָ��
typedef int (*NB_AutoInitModule)(NB_Handle);

//��ȡNBģ�����Ϣ�����磺Manufacturer ID, Device Module,Firmware Version,Freqenucy band
typedef int (*NB_ModuleInfo)(NB_Handle);

//��ȡNBģ�������ע����Ϣ
typedef int (*NB_NetRegisterInfo)(NB_Handle);

//��ȡNBģ��SIM ������Ϣ
typedef const char* (*NB_USIMInfo)(NB_Handle);

//��ȡNBģ���ź�ǿ��
typedef int (*NB_NetSign)(NB_Handle);

//����NBģ��ͨ��������
typedef int (*NB_DefPDP)(NB_Handle);

//ʧ��NBģ����������Ϣ
typedef int (*NB_DeactPDP)(NB_Handle);

//����UDP
typedef int (*NB_CreateUdp)(NB_Handle);

//�ر�UDP
typedef int (*NB_CloseUdp)(NB_Handle);

//����UDP����
typedef int (*NB_SendUdpData)(NB_Handle,int,char*);


//CoAP ��Ϣ
typedef int (*NB_CoAPServer)(NB_Handle, Bool,char*);

//CoAP ������ʾ��ʽ����
typedef int (*NB_CoAPSentIndication)(NB_Handle,int code);

//CoAP �������ݷ�ʽ����
typedef int (*NB_CoAPReceMode)(NB_Handle,int);

//CoAPЭ����Ϣ����
typedef int (*NB_CoAPSentMsg)(NB_Handle,int,char*);

//NB Modle reset cmd
typedef int (*NB_Reset)(NB_Handle);

//NB ping function
typedef int (*NB_Ping)(NB_Handle);

//�жϽ��������ǲ���Udp
//typedef int (*NB_isUdpReceData)(NB_Handle,int,char*);

//�жϽ��������Ƿ�ΪCoAP����
//typedef int (*NB_IsCoAPReceData)(NB_Handle,int,char*);


//����UDP����
//typedef int (*NB_ReceUdpData)(NB_Handle);

//CoAPЭ����Ϣ����
//typedef int (*NB_CoAPReceMsg)(NB_Handle);

typedef int (*NB_MainThread)(NB_Handle);
//******************************************************************************
// Name : NB_FxnTable
//
// brief : ����NBIOTģ�����Ҫʵ�ֵĹ��ܺ���
//
typedef struct
{
  NB_Open                 openFxn;
  NB_AutoInitModule       nbModuleInit;
  NB_ModuleInfo           getModuleInfo;
  NB_NetRegisterInfo      getModuleRegisterInfo;
  NB_USIMInfo             getUSIMinfo;
  NB_NetSign              getSign;
//  NB_DefPDP               definePDP;
//  NB_DeactPDP             deactPDP;
  NB_CreateUdp            createUdp;
  NB_CloseUdp             closeUdp;
  NB_SendUdpData          sendUdp;
//  
  NB_CoAPServer           coapServer;
  NB_CoAPSentIndication   coapSentIndication;
  NB_CoAPReceMode         coapSetReceMode;
  NB_CoAPSentMsg          coapSentMsg;
//  
//  NB_Reset                nbReset;
//  NB_Ping                 nbPing;
//  
//  NB_Close                closeFxn;
  NB_MainThread           mainThread;
}NB_FxnTable;

//*****************************************************************************
// ������Դ����
typedef void (*NB_PwrInit)(void);    //��ʼ��NBģ��ʹ������
typedef void (*NB_PwrOnOff)(char);   //ʹ�ܻ�ʧ��NBģ���Դ����
typedef struct
{
  NB_PwrInit  nbPwrInit;
  NB_PwrOnOff nbPwrOnOff;
}NB_Pwr;

typedef int  (*NB_ReceCB)(msg_types_t,int ,char*);

typedef int  (*NB_Log)(char*,int);
//******************************************************************************
// Name : NB_Config
//
// brief : NBIOTģ��Ĺ��ܽṹ����
//
typedef struct NB_Config
{
  NB_FxnTable        *fxnTablePtr;
  void               *object;         //NB uart����ָ��
  NB_ReceCB          AppReceCB;
  const NB_Pwr       *nbPwr;
  NB_Log             log;
  
}NB_Config;


//******************************************************************************
// function declare
//******************************************************************************

//******************************************************************************
// fn : NBModule_open
//
// brief : ��ʼ����֮��ص�Ӳ��
//
// param : handle -> �û������NB_Config����ָ��
//
// return : none
extern uint8 NBModule_open(NB_Handle handle);

//******************************************************************************
// fn : NBModule_Init
//
// brief : ��ʼ��NBģ�����缰��ȡһЩ��Ϣ
//
// param : none
//
// return : none
extern uint8 NBModule_Init(NB_Handle handle);
//******************************************************************************
// fn : NB_ModuleInfo
//
// brief : ��ȡNBģ�����Ϣ�����磺Manufacturer ID, Device Module,
//         Firmware Version,Freqenucy band
//
// param : none
//
// return : none
extern uint8 NBModule_Info(NB_Handle handle);

//******************************************************************************
// fn : NBModule_isRegister
//
// brief : ��ѯNBģ���Ƿ�ע������
//
// param : none
//
// return : none
extern uint8 NBModule_isRegister(NB_Handle handle);
//******************************************************************************
// fn : NBModule_IMSI
//
// brief : ��ȡģ��IMSI��
//
// param : none
//
// return : none
extern const char* NBModule_IMSI(NB_Handle handle);

//******************************************************************************
// fn : NBModule_Sign
//
// brief : ��ȡ��ǰ�ź�ֵ
//
// param : none
//
// return : none
extern uint8 NBModule_Sign(NB_Handle handle);

//******************************************************************************
// fn : NBModule_ActPDP
//
// brief : ���嵱ǰNBģ��ͨ��������
//
// param : none
//
// return : none
extern uint8 NBModule_ActPDP(NB_Handle handle);

//******************************************************************************
// fn : NBModule_DeactPDP
//
// brief : ʧ��ģ��������
//
// param : none
//
// return : none
extern uint8 NBModule_DeactPDP(NB_Handle handle);

//******************************************************************************
// fn : NBModule_CreateUDP
//
// brief : ����UDP
//
// param : none
//
// return : none
extern uint8 NBModule_CreateUDP(NB_Handle handle);

//******************************************************************************
// fn : NBModule_CloseUDP
//
// brief : �ر�ָ����udp
//
// param : handle -> NB �ṹ��Ϣָ��
//         socketId -> �رյ�socket id��
//
// return : none
extern uint8 NBModule_CloseUDP(NB_Handle handle);

//******************************************************************************
// fn : NBModule_SendData
//
// brief : ��udp��������
//
// param : handle -> NB �ṹ��Ϣָ��
//         len -> ��Ϣ����
//         msg -> ��Ϣ
//
// return : none
extern uint8 NBModule_SendData(NB_Handle handle,int len,char* msg);

//******************************************************************************
// fn : NBModule_CoAPServer
//
// brief : �������ѯNBģ�鵱ǰCoAP��������Ϣ
//
// param : handle -> NB �ṹ��Ϣָ��
//         isSet  -> true -> write,
//                   false-> read
//
// return : none
extern uint8 NBModule_CoAPServer(NB_Handle handle,Bool isSet,char*coap);

//******************************************************************************
// fn : NBModule_CoAPSentIndication
//
// brief : ����CoAP������Ϣ����ָʾ��ʽ
//
// param : handle -> NB �ṹ��Ϣָ��
//         code   -> 0  �����κη�Ӧ
//                   !0 ��Ӧ��Ϣ
//
// return : none
extern uint8 NBModule_CoAPSentIndication(NB_Handle handle,int code);

//******************************************************************************
// fn : NBModule_CoAPReceIndication
//
// brief : ����CoAP������Ϣ��ʾ��ʽ
//
// param : handle -> NB �ṹ��Ϣָ��
//         code   -> 0  code -> 0��ʾ���棬1 ��ʾֱ�ӽ��ա���Ŀǰ��֧������ģʽ��
//
// return : none
extern uint8 NBModule_CoAPReceIndication(NB_Handle handle,int code);

//******************************************************************************
// fn : NBModule_CoAPSendMsg
//
// brief : ����COAP������Ϣ��COAP������
//
// param : handle -> NB �ṹ��Ϣָ��
//         len    -> ��Ϣ����
//         msg    -> ��Ϣ����
//
// return : none
extern uint8 NBModule_CoAPSendMsg(NB_Handle handle,int len,char*msg);
//******************************************************************************
// fn : NBModule_CoAPSendMsg
//
// brief : ����COAP������Ϣ��COAP������
//
// param : handle -> NB �ṹ��Ϣָ��
//
// return : none
extern void NBModule_Reset(NB_Handle handle);
//******************************************************************************
// fn : NBModule_Ping
//
// brief : ping remote address
//
// param : handle -> NB �ṹ��Ϣָ��
//         ipAddr -> Զ��ip��ַ.��118.29.240.46
//
// return : none
extern void NBModule_Ping(NB_Handle handle,char *ipAddr);

//******************************************************************************
// fn : NBModule_Main
//
// brief : ѭ������������
//
// param : handle -> NB �ṹ��Ϣָ��
//
// return : none
extern int NBModule_Main(NB_Handle handle);

#ifdef __cplusplus
}
#endif

#endif    //NB_BOARD_H
