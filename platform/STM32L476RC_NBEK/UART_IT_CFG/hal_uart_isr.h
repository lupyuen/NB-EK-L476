//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : hal_uart_isr.h
//  Date :     2018-11-20 13:49
//  Version :  V0001
//  History :  初始创建版本
//******************************************************************************
#ifndef _HAL_UART_ISR_H
#define _HAL_UART_ISR_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "stm32l4xx_hal.h"
  
//******************************************************************************
// 为节省内存使用，这里使用了小内存。用户可以根据自己的需求进行更改
//
#define   RECE_BUF_MAX_LEN       512
#define   SENT_BUF_MAX_LEN       128 
  
/* UART Events */
#define HAL_UART_RX_FULL         0x01
#define HAL_UART_RX_TIMEOUT      0x02
  
typedef struct
{
  uint8_t  rxBuf[RECE_BUF_MAX_LEN];
  uint16_t rxHead;
  uint16_t rxTail;

  uint8_t  rxTick;
  uint32_t rxShdw;

  uint8_t txBuf[2][SENT_BUF_MAX_LEN];
  uint8_t txId[2];
  uint8_t txSel;
  
  uint8_t   txTick;
  volatile uint32_t txShdw;
  volatile uint8_t  txShdwValid; // TX shadow value is valid
  uint8_t txPending;
}uartISRCfg_t;
  
//******************************************************************************
// fn : UartIsr_Init
//
// brief : 初始化uartISRCfg_t的结构
//
// param : none
//
// return : none
extern void UartIsr_Init(UART_HandleTypeDef*);

//******************************************************************************
// fn : UartIsr_GetBuf
//
// brief : 返回接收缓存地址
//
// param : none
//
// return : 地址
extern uint8_t* UartIsr_GetBuf(void);

//******************************************************************************
// fn :     UartIsr_Read
//
// brief :  从接收缓存里读取指定长度串口数据，并释放占用的空间
//
// param :  buf  - 有效长度的目标缓存
//          len  - 要读取的长度
//
// return : length of buffer that was read
//******************************************************************************
extern uint16_t UartIsr_Read(uint8_t *buf, uint16_t len);

//******************************************************************************
// fn :     UartIsr_Write
//
// brief :  将要发送的数据，拷贝到发送缓存中
//
// param :  buf  - 有效长度的目标缓存
//          len  - 要读取的长度
//
// return : length of buffer that was write
//******************************************************************************
extern uint16_t UartIsr_Write(uint8_t *buf, uint16_t len);

//**************************************************************************************************
// fn  :  UartIsr_Avail
//
// brief :  计算缓存区中数据个数 - the number of bytes in the buffer.
//
// param :  none
//
// return : the number of bytes
//**************************************************************************************************/
extern uint16_t UartIsr_Avail(void);

//******************************************************************************
// fn : UartIsr_Poll
//
// brief : 轮询接收缓存数据长度
//
// param : none
//
// return : none
//****************************************************************************/
extern uint8_t UartIsr_Poll(void);  
//******************************************************************************
// fn : UARTIT_RxCplt
//
// brief : 接收完成回调
//
// param : none
//
// return : none
extern void UartIsr_RxCplt(UART_HandleTypeDef* handle);
//******************************************************************************
// fn : UartIsr_TxCplt
//
// brief : 发送完成回调
//
// param : none
//
// return : none
//****************************************************************************/
extern void UartIsr_TxCplt(UART_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif

#endif   //_HAL_UART_ISR_H 
