//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_bsp_nbuart.h
//  Date     : 2018-11-06 14:06
//  Version  : V0001
// ��ʷ��¼  : ��һ�δ���
//******************************************************************************
#ifndef _LOS_BSP_NBUART_H
#define _LOS_BSP_NBUART_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32l4xx_hal.h"
#include "stm32l4xx_nbek.h"
#include "NB_BC95.h"


//******************************************************************************
// fn : LOS_EvbNBUartInit
//
// brief : Init the NBUart
//
// param : none
//
// return : none
extern void LOS_EvbNBUartInit(void);

//******************************************************************************
// fn : LOS_EvbNBUartDMAInit
//
// brief : Init the NBUart
//
// param : cb-> NB���մ�������ָ��
//         baud -> �ݲ�ʹ��
//
// return : none
extern void LOS_EvbNBUartDMAInit(bc95_receive_cb cb,uint32_t baud);

//******************************************************************************
// fn : LOS_EvbNBUartWrite
//
// brief : ͨ���������ⷢ��ָ������
//
// param : buf -> ���ݻ����ַ
//         len -> ���ݳ���
//
// return : none
extern void LOS_EvbNBUartWrite(uint8_t *buf,uint16_t len);

//******************************************************************************
// fn : LOS_EvbNBUartClose
//
// brief : Close the uart peripheral
//
// param : none
//
// return : none
extern void LOS_EvbNBUartClose(void);


//******************************************************************************
// fn : LOS_EvbNBUartPoll
//
// brief : ��ѯ�����¼�
//
// param : none
//
// return : �¼�
extern uint8_t LOS_EvbNBUartPoll(void);



#ifdef __cplusplus
}
#endif

#endif
