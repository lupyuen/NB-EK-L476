//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : _dma_cfg.h
//  Date     : 2017-12-04 16:11
//  Version  : V0001
// ��ʷ��¼  : ��һ�δ���
//******************************************************************************
#ifndef _DMA_CFG_H
#define _DMA_CFG_H

#ifdef __cplusplus
extern "C"
{
#endif
  
#include "stm32l4xx_hal.h"
#include "los_typedef.h"  
//******************************************************************************
// Ϊ��ʡ�ڴ�ʹ�ã�����ʹ����С�ڴ档�û����Ը����Լ���������и���
//
#define   RECE_BUF_MAX_LEN       256
#define   SENT_BUF_MAX_LEN       128
  
/* UART Events */
#define HAL_UART_RX_FULL         0x01
#define HAL_UART_RX_TIMEOUT      0x02

//******************************************************************************
// Name : dmaReceCfg
//
// brief : ���崮����DMA��ʽ����ʱ�����ýṹ��
typedef struct dmaReceCfg
{
  uint16_t  buf[RECE_BUF_MAX_LEN];// ������
  uint16_t  rxHead;               // ��Ч���ݿ�ʼͷ
  uint16_t  rxTail;               // ���һ������δβ
  uint8_t   rxTick;               // ֡��ʱʱ�䣬��λ��ms
  uint32_t  rxShdw;               // ��ʱ������ʼ��ֵ
  
  uint8_t   txBuf[2][SENT_BUF_MAX_LEN];
  uint8_t   txIdx[2];
  uint8_t   txSel;
  
  uint8_t   txTick;
  volatile uint32_t txShdw;
  volatile uint8_t  txShdwValid; // TX shadow value is valid
  uint8_t   txDMAPending;        // UART TX DMA is pending
  
}dmaCfg_t;


//******************************************************************************
//������������
//
typedef void (*sendData_cb)(uint8_t* buf,uint16_t len); 

//******************************************************************************
// fn : UartDma_Init
//
// brief : ��ʼ��dmaReceCfg�Ľṹ
//
// param : none
//
// return : the address of rx Buffer
extern uint8_t* UartDma_Init(sendData_cb,USART_TypeDef*);

//******************************************************************************
// fn :     UartDma_Read
//
// brief :  �ӽ��ջ������ȡָ�����ȴ������ݣ����ͷ�ռ�õĿռ�
//
// param :  buf  - ��Ч���ȵ�Ŀ�껺��
//          len  - Ҫ��ȡ�ĳ���
//
// return : length of buffer that was read
//******************************************************************************
extern uint16_t UartDma_Read(uint8_t *buf, uint16_t len);

//******************************************************************************
// fn :     UartDma_Write
//
// brief :  ��Ҫ���͵����ݣ����������ͻ�����
//
// param :  buf  - ��Ч���ȵ�Ŀ�껺��
//          len  - Ҫ��ȡ�ĳ���
//
// return : length of buffer that was write
//******************************************************************************
extern uint16_t UartDma_Write(uint8_t *buf, uint16_t len);
//**************************************************************************************************
// fn  :  UartDma_Avail
//
// brief :  ���㻺���������ݸ��� - the number of bytes in the buffer.
//
// param :  none
//
// return : the number of bytes
//**************************************************************************************************/
extern uint16_t UartDma_Avail(void);

//******************************************************************************
// fn : UartDma_Poll
//
// brief : ��ѯ���ջ������ݳ���
//
// param : none
//
// return : none
//****************************************************************************/
extern uint8_t UartDma_Poll(void);

//******************************************************************************
// fn : UartDma_TxCpltCB
//
// brief : ������ɻص�
//
// param : none
//
// return : none
//****************************************************************************/
extern void UartDma_TxCpltCB(UART_HandleTypeDef *huart);
#ifdef __cplusplus
}
#endif

#endif   //_DMA_RECE_CFG_H
