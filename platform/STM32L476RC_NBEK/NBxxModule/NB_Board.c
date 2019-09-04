//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : NB_Board.c
//  Date     : 2017-12-11 11:18
//  Version  : V0001
// ��ʷ��¼  : ��һ�δ���
//******************************************************************************
#include "NB_Board.h"


//==============================================================================
extern const NB_FxnTable BC95_FxnTable;

//��������
//******************************************************************************
// fn : NBModule_open
//
// brief : ��ʼ����֮��ص�Ӳ��
//
// param : none
//
// return : none
extern Bool NBModule_open(NB_Handle handle)
{
  if(handle == null)
  {
    return FALSE;
  }
  handle->fxnTablePtr = (void*)&BC95_FxnTable;
  
  //����Ӧ��Ӳ��
  handle->fxnTablePtr->openFxn(handle);
  
  return TRUE;
}
//******************************************************************************
// fn : NBModule_Init
//
// brief : ��ʼ��NBģ�����缰��ȡһЩ��Ϣ
//
// param : none
//
// return : none
extern Bool NBModule_Init(NB_Handle handle)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->nbModuleInit == null)
  {
    return FALSE;
  }
  
  //����
  handle->fxnTablePtr->nbModuleInit(handle);
  return TRUE;
}
//******************************************************************************
// fn : NB_ModuleInfo
//
// brief : ��ȡNBģ�����Ϣ�����磺Manufacturer ID, Device Module,
//         Firmware Version,Freqenucy band
//
// param : none
//
// return : none
uint8 NBModule_Info(NB_Handle handle)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->getModuleInfo == null)
  {
    return FALSE;
  }
  
  handle->fxnTablePtr->getModuleInfo(handle);
  
  return TRUE;
}
//******************************************************************************
// fn : NBModule_isRegister
//
// brief : ��ѯNBģ���Ƿ�ע������
//
// param : none
//
// return : none
uint8 NBModule_isRegister(NB_Handle handle)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->getModuleRegisterInfo == null)
  {
    return FALSE;
  }
  
  return handle->fxnTablePtr->getModuleRegisterInfo(handle);

}
//******************************************************************************
// fn : NBModule_IMSI
//
// brief : ��ȡģ��IMSI��
//
// param : none
//
// return : none
const char* NBModule_IMSI(NB_Handle handle)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->getUSIMinfo == null)
  {
    return FALSE;
  }
  return handle->fxnTablePtr->getUSIMinfo(handle);
}

//******************************************************************************
// fn : NBModule_Sign
//
// brief : ��ȡ��ǰ�ź�ֵ
//
// param : none
//
// return : TRUE-> ָ����ִ�У�FALSE->ִ�й����з�������
// ע�⣺
// �߱����ź�ֵ����ͨ����Ϣ�첽���ص�������
extern uint8 NBModule_Sign(NB_Handle handle)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->getSign == null)
  {
    return FALSE;
  }
  return handle->fxnTablePtr->getSign(handle);
}
//******************************************************************************
// fn : NBModule_CreateUDP
//
// brief : ����UDP
//
// param : none
//
// return : none
uint8 NBModule_CreateUDP(NB_Handle handle)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->createUdp == null)
  {
    return FALSE;
  }
  
  return handle->fxnTablePtr->createUdp(handle);
}
//******************************************************************************
// fn : NBModule_CloseUDP
//
// brief : �ر�ָ����udp
//
// param : handle -> NB �ṹ��Ϣָ��
//         socketId -> �رյ�socket id��
//
// return : none
uint8 NBModule_CloseUDP(NB_Handle handle)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->closeUdp == null)
  {
    return FALSE;
  }
  return handle->fxnTablePtr->closeUdp(handle);
}
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
uint8 NBModule_SendData(NB_Handle handle,int len,char* msg)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->sendUdp == null)
  {
    return FALSE;
  }
  return handle->fxnTablePtr->sendUdp(handle,len,msg);
}

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
uint8 NBModule_CoAPServer(NB_Handle handle,Bool isSet,char*coap)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->coapServer == null)
  {
    return FALSE;
  }
  return handle->fxnTablePtr->coapServer(handle,isSet,coap);
}

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
uint8 NBModule_CoAPSentIndication(NB_Handle handle,int code)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->coapSentIndication == null)
  {
    return FALSE;
  }
  return handle->fxnTablePtr->coapSentIndication(handle,code);
}

//******************************************************************************
// fn : NBModule_CoAPReceIndication
//
// brief : ����CoAP������Ϣ��ʾ��ʽ
//
// param : handle -> NB �ṹ��Ϣָ��
//         code   -> 0  code -> 0��ʾ���棬1 ��ʾֱ�ӽ��ա���Ŀǰ��֧������ģʽ��
//
// return : none
uint8 NBModule_CoAPReceIndication(NB_Handle handle,int code)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->coapSetReceMode == null)
  {
    return FALSE;
  }
  return handle->fxnTablePtr->coapSetReceMode(handle,code);
}

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
uint8 NBModule_CoAPSendMsg(NB_Handle handle,int len,char*msg)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->coapSentMsg == null)
  {
    return FALSE;
  }
  return handle->fxnTablePtr->coapSentMsg(handle,len,msg);
}
//******************************************************************************
// fn : NBModule_Main
//
// brief : ѭ������������
//
// param : handle -> NB �ṹ��Ϣָ��
//
// return : none
extern int NBModule_Main(NB_Handle handle)
{
  if(handle == null)
  {
    return FALSE;
  }
  if(handle->fxnTablePtr->mainThread == null)
  {
    return FALSE;
  }
  
  //����
  return handle->fxnTablePtr->mainThread(handle);

}
