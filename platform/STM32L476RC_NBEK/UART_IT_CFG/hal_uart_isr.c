//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : hal_uart_isr.c
//  Date :     2018-11-20 13:54
//  Version :  V0001
//  History :  初始创建版本
//*****************************************************************************
#include "hal_uart_isr.h"
#include "los_mux.h"
#include "usart.h"





#define HAL_UART_ISR_IDLE     (1)   //1ms


#define HAL_UART_ISR_FULL     (RECE_BUF_MAX_LEN - 16)

#ifndef TRUE
#define TRUE                  1
#endif
#ifndef FALSE
#define FALSE                 0
#endif
#define HAL_UART_ISR_RX_AVAIL() \
  (isrCfg.rxTail >= isrCfg.rxHead) ? \
  (isrCfg.rxTail - isrCfg.rxHead) : \
  (RECE_BUF_MAX_LEN - isrCfg.rxHead + isrCfg.rxTail)

  
//------------------------------------------------------------------------------
// Declare and Define
//
extern const UART_Config gpsUartCfg;   
static uartISRCfg_t      isrCfg;
UART_HandleTypeDef       *hUart;
//******************************************************************************
// fn : UartIsr_Init
//
// brief : 初始化dmaReceCfg的结构
//
// param : none
//
// return : the address of rx Buffer
void UartIsr_Init(UART_HandleTypeDef*huart)
{
  isrCfg.rxHead = 0;
  isrCfg.rxTail = 0;
  isrCfg.rxTick = 0;
  isrCfg.rxShdw = 0;
  
  isrCfg.txId[0] = 0;
  isrCfg.txId[1] = 0;
  isrCfg.txSel = 0;
  isrCfg.txTick = HAL_UART_ISR_IDLE;
  //创建互斥锁
  //LOS_MuxCreate(&isrCfg.isrMutex);
  hUart = huart;
}
//******************************************************************************
// fn : UartIsr_GetBuf
//
// brief : 返回接收缓存地址
//
// param : none
//
// return : 地址
uint8_t* UartIsr_GetBuf(void)
{
  return isrCfg.rxBuf;
}
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
uint16_t UartIsr_Read(uint8_t *buf, uint16_t len)
{
  uint16_t cnt = 0;
  UINTPTR uvIntSave;
  uvIntSave = LOS_IntLock();
  
  while ((isrCfg.rxHead != isrCfg.rxTail) && (cnt < len))
  {
    *buf++ = isrCfg.rxBuf[isrCfg.rxHead++];
    if (isrCfg.rxHead >= RECE_BUF_MAX_LEN)
    {
      isrCfg.rxHead = 0;
    }
    cnt++;
  }
  
  LOS_IntRestore(uvIntSave);
  return cnt;
}
//***************************************************************************************************
// fn : UartIsr_Write
//
// brief : Send data with usart
//
// param : buf -> the reference of data
//         len -> the length
//
// return : len
uint16_t UartIsr_Write(uint8_t *buf, uint16_t len)
{
  uint16_t cnt = 0;
  uint8_t txSel = 0,txIdx = 0;
  UINTPTR uvIntSave;
  
  //write the data to buf
  // Enforce all or none.
  if ((len + isrCfg.txId[isrCfg.txSel]) > SENT_BUF_MAX_LEN)
  {
    return 0;
  }
  uvIntSave = LOS_IntLock();
  
  txSel = isrCfg.txSel;
  txIdx = isrCfg.txId[txSel];
  LOS_IntRestore(uvIntSave); 
  
  for (cnt = 0; cnt < len; cnt++)
  {
    isrCfg.txBuf[txSel][txIdx++] = buf[cnt];
  }

  uvIntSave = LOS_IntLock();
  if (txSel != isrCfg.txSel)
  {
    LOS_IntRestore(uvIntSave);
    
    txSel = isrCfg.txSel;
    txIdx = isrCfg.txId[txSel];

    for (cnt = 0; cnt < len; cnt++)
    {
      isrCfg.txBuf[txSel][txIdx++] = buf[cnt];
    }
    uvIntSave = LOS_IntLock();
  }

  isrCfg.txId[txSel] = txIdx;

  if (isrCfg.txId[(txSel ^ 1)] == 0)
  {
    // TX is expected to be fired
    isrCfg.txPending = TRUE;
  }
  
  LOS_IntRestore(uvIntSave);
  return cnt;
}
//**************************************************************************************************
// fn  :  UartIsr_Avail
//
// brief :  计算缓存区中数据个数 - the number of bytes in the buffer.
//
// param :  none
//
// return : the number of bytes
//**************************************************************************************************/
uint16_t UartIsr_Avail(void)
{
  return HAL_UART_ISR_RX_AVAIL();
}

//******************************************************************************
// fn : UartIsr_Poll
//
// brief : 轮询接收缓存数据长度
//
// param : none
//
// return : none
//****************************************************************************/
uint8_t UartIsr_Poll(void)
{
  uint8_t evt = 0;
  uint16_t cnt = 0;
  volatile uint16_t tail = RECE_BUF_MAX_LEN - hUart->RxXferCount;

  if(isrCfg.rxHead != tail)
  {
    if(tail != isrCfg.rxTail)
    {
      isrCfg.rxTail = tail;
      
      if(isrCfg.rxTick == 0)
      {
        isrCfg.rxShdw = LOS_TickCountGet();
      }
      isrCfg.rxTick = HAL_UART_ISR_IDLE;
    }
    else if(isrCfg.rxTick)
    {
      uint32_t Tick = LOS_TickCountGet();
      
      uint32_t delta = Tick >= isrCfg.rxShdw ?
                       (Tick - isrCfg.rxShdw ): 
                       (Tick + (UINT32_MAX - isrCfg.rxShdw));

      if (isrCfg.rxTick > delta)
      {
        isrCfg.rxTick -= delta;
      }
      else
      {
        isrCfg.rxTick = 0;
      }
    }
    cnt = UartIsr_Avail();
  }
  else
  {
    isrCfg.rxTick = 0;
  }
  
  if (isrCfg.txShdwValid)
  {
    uint32_t decr = LOS_TickCountGet()- isrCfg.txShdw;;
	
    if (decr > isrCfg.txTick)
    {
      // No protection for txShdwValid is required
      // because while the shadow was valid, DMA ISR cannot be triggered
      // to cause concurrent access to this variable.
      isrCfg.txShdwValid = FALSE;
    }
  }
  
  if (isrCfg.txPending && !isrCfg.txShdwValid)
  {
    // Clear the pending flag
    isrCfg.txPending = FALSE;
    //Send data

    Board_UART_Send(&gpsUartCfg,isrCfg.txBuf[isrCfg.txSel],isrCfg.txId[isrCfg.txSel]);
    isrCfg.txSel ^= 1;
  }
  
  if (cnt >= HAL_UART_ISR_FULL)
  {
    evt = HAL_UART_RX_FULL;
  }
  else if (cnt && !isrCfg.rxTick)
  {
    evt = HAL_UART_RX_TIMEOUT;
  }
  return evt;
}
//******************************************************************************
// fn : UARTIT_RxCplt
//
// brief : callback
//
// param : none
//
// return :none
void UartIsr_RxCplt(UART_HandleTypeDef* handle)
{
  Board_UART_Rece(&gpsUartCfg,isrCfg.rxBuf,RECE_BUF_MAX_LEN);
}
//******************************************************************************
// fn : UartIsr_TxCplt
//
// brief : 发送完成回调
//
// param : none
//
// return : none
//****************************************************************************/
extern void UartIsr_TxCplt(UART_HandleTypeDef *huart)
{

  // Indicate that the other buffer is free now.
  isrCfg.txId[(isrCfg.txSel ^ 1)] = 0;
  
  // Set TX shadow
  isrCfg.txShdw = LOS_TickCountGet();
  isrCfg.txShdwValid = TRUE;

  // If there is more Tx data ready to go, re-start the DMA immediately on it.
  if (isrCfg.txId[isrCfg.txSel])
  {
    // UART TX DMA is expected to be fired
    isrCfg.txPending = TRUE;
  }
}
