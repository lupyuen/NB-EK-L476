//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : _dma_cfg.c
//  Date     : 2017-12-18 20:43
//  Version  : V0001
// ��ʷ��¼  : ��һ�δ���
//******************************************************************************
#include <string.h>
#include "_dma_cfg.h"
#include "los_mux.h"


#define HI_UINT16(DATA)       ((DATA)>>8)
#define LO_UINT16(DATA)       ((DATA) & 0xFF)
#define BUILD_UINT16(hi,lo)   (uint16_t)(((hi) & 0x00FF) << 8 + ((lo) & 0x00FF))

#define DMA_NEW_RX_BYTE(IDX)  (0x00 == HI_UINT16(dmaCfg.buf[(IDX)]))
#define DMA_GET_RX_BYTE(IDX)  (LO_UINT16(dmaCfg.buf[(IDX)]))
#define DMA_CLR_RX_BYTE(IDX)  (dmaCfg.buf[(IDX)] = 0xFFFF)

#define HAL_UART_DMA_IDLE     (5)   //1ms


#define HAL_UART_DMA_FULL     (RECE_BUF_MAX_LEN - 16)

#ifndef TRUE
#define TRUE                  1
#endif

#ifndef FALSE
#define FALSE                 0
#endif
static dmaCfg_t       dmaCfg;
static sendData_cb    dmaSendCb;
static USART_TypeDef* hDmaUart = 0;
//*****************************************************************************
// fn :    findTail
//
// brief :  �ҵ���ǰDMA���ջ��������ڲ�����λ��
//
// param : None.
//
// return : Index of tail of rxBuf.
//*****************************************************************************/
static uint16_t findTail(void)
{
  uint16_t idx = dmaCfg.rxHead;

  do
  {
    if (!DMA_NEW_RX_BYTE(idx))
    {
      break;
    }
    
    if (++idx >= RECE_BUF_MAX_LEN)
    {
      idx = 0;
    }
  } while (idx != dmaCfg.rxHead);

  return idx;
}

//******************************************************************************
// fn : UartDma_Init
//
// brief : ��ʼ��dmaReceCfg�Ľṹ
//
// param : none
//
// return : none
uint8_t* UartDma_Init(sendData_cb sendCb ,USART_TypeDef* hUart)
{
  memset(dmaCfg.buf,0xff,RECE_BUF_MAX_LEN<<1);
  dmaCfg.rxHead   = 0;
  dmaCfg.rxTail   = 0;
  dmaCfg.rxTick   = 0;
  dmaCfg.rxShdw   = 0;
  dmaCfg.txSel    = 0;
  
  dmaCfg.txIdx[0] = 0;
  dmaCfg.txIdx[1] = 0;
  
  dmaCfg.rxTick   = 0;   //delay 1ms
  
  dmaCfg.txDMAPending = FALSE;
  dmaCfg.txShdwValid  = FALSE;
  
  //����������
  //LOS_MuxCreate(&dmaCfg.dmaMutex);
  
  dmaSendCb = sendCb;
  
  hDmaUart = hUart;
  return (uint8_t*)dmaCfg.buf;
}
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
uint16_t UartDma_Read(uint8_t *buf, uint16_t len)
{
  uint16_t cnt;
  UINTPTR uvIntSave;
  uvIntSave = LOS_IntLock();
  for (cnt = 0; cnt < len; cnt++)
  {
    if (!DMA_NEW_RX_BYTE(dmaCfg.rxHead))
    {
      break;
    }
    *buf++ = DMA_GET_RX_BYTE(dmaCfg.rxHead);
    
    //�ͷ�ռ�ÿռ�
    DMA_CLR_RX_BYTE(dmaCfg.rxHead);

    if (++(dmaCfg.rxHead) >= RECE_BUF_MAX_LEN)
    {
      dmaCfg.rxHead = 0;
    }
  }
  LOS_IntRestore(uvIntSave);
  return cnt;
}
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
uint16_t UartDma_Write(uint8_t *buf, uint16_t len)
{
  uint16_t cnt;
  uint8_t txSel;
  uint8_t txIdx;
  UINTPTR uvIntSave;
  // Enforce all or none.
  if ((len + dmaCfg.txIdx[dmaCfg.txSel]) > SENT_BUF_MAX_LEN)
  {
    return 0;
  }
  uvIntSave = LOS_IntLock();
  
  txSel = dmaCfg.txSel;
  txIdx = dmaCfg.txIdx[txSel];
  LOS_IntRestore(uvIntSave); 
  
  for (cnt = 0; cnt < len; cnt++)
  {
    dmaCfg.txBuf[txSel][txIdx++] = buf[cnt];
  }

  uvIntSave = LOS_IntLock();
  if (txSel != dmaCfg.txSel)
  {
    LOS_IntRestore(uvIntSave);
    
    txSel = dmaCfg.txSel;
    txIdx = dmaCfg.txIdx[txSel];

    for (cnt = 0; cnt < len; cnt++)
    {
      dmaCfg.txBuf[txSel][txIdx++] = buf[cnt];
    }
    uvIntSave = LOS_IntLock();
  }

  dmaCfg.txIdx[txSel] = txIdx;

  if (dmaCfg.txIdx[(txSel ^ 1)] == 0)
  {
    // TX DMA is expected to be fired
    dmaCfg.txDMAPending = TRUE;
  }
  LOS_IntRestore(uvIntSave);
  return cnt;
}
//**************************************************************************************************
// fn  :  UartDma_Avail
//
// brief :  ���㻺���������ݸ��� - the number of bytes in the buffer.
//
// param :  none
//
// return : the number of bytes
//**************************************************************************************************/
extern uint16_t UartDma_Avail(void)
{
  uint16_t cnt = 0;

  if (DMA_NEW_RX_BYTE(dmaCfg.rxHead))
  {
    uint16_t idx;

    for (idx = 0; idx < RECE_BUF_MAX_LEN; idx++)
    {
      if (DMA_NEW_RX_BYTE(idx))
      {
        cnt++;
      }
    }
  }

  return cnt;
}
//******************************************************************************
// fn : UartDma_Poll
//
// brief : ��ѯ���ջ������ݳ���
//
// param : none
//
// return : none
//****************************************************************************/
uint8_t UartDma_Poll(void)
{
  uint16_t cnt = 0;
  uint8_t evt = 0;

  if(DMA_NEW_RX_BYTE(dmaCfg.rxHead))
  {
    uint16_t tail = findTail();
    
    // If the DMA has transferred in more Rx bytes, reset the Rx idle timer.
    if (dmaCfg.rxTail != tail)
    {
      dmaCfg.rxTail = tail;

      if (dmaCfg.rxTick == 0)
      {
        dmaCfg.rxShdw = LOS_TickCountGet();
      }
      
      dmaCfg.rxTick = HAL_UART_DMA_IDLE;
    }
    else if (dmaCfg.rxTick)
    {
      uint32_t Tick = LOS_TickCountGet();
      uint32_t delta = Tick >= dmaCfg.rxShdw ?
                               (Tick - dmaCfg.rxShdw ): 
                               (Tick + (UINT32_MAX - dmaCfg.rxShdw));
      
      if (dmaCfg.rxTick > delta)
      {
        dmaCfg.rxTick -= delta;
        dmaCfg.rxShdw = Tick;
      }
      else
      {
        dmaCfg.rxTick = 0;
      }
    }
    cnt = UartDma_Avail();
  }
  else
  {
    dmaCfg.rxTick = 0;
  }

  if (cnt >= HAL_UART_DMA_FULL)
  {
    evt = HAL_UART_RX_FULL;
  }
  else if (cnt && !dmaCfg.rxTick)
  {
    evt = HAL_UART_RX_TIMEOUT;
  }
  
  if (dmaCfg.txShdwValid)
  {
    uint32_t decr = LOS_TickCountGet()- dmaCfg.txShdw;;
	
    if (decr > dmaCfg.txTick)
    {
      // No protection for txShdwValid is required
      // because while the shadow was valid, DMA ISR cannot be triggered
      // to cause concurrent access to this variable.
      dmaCfg.txShdwValid = FALSE;
    }
  }
  
  if (dmaCfg.txDMAPending && !dmaCfg.txShdwValid)
  {
    // Clear the DMA pending flag
    dmaCfg.txDMAPending = FALSE;
    //Send  data
    if(dmaSendCb)
    {
      dmaSendCb(dmaCfg.txBuf[dmaCfg.txSel],dmaCfg.txIdx[dmaCfg.txSel]);
    }
    dmaCfg.txSel ^= 1;
  }

  return evt;
}
//******************************************************************************
// fn : UartDma_TxCpltCB
//
// brief : ������ɻص�
//
// param : none
//
// return : none
//****************************************************************************/
extern void UartDma_TxCpltCB(UART_HandleTypeDef *huart)
{
  if(huart->Instance == hDmaUart)
  {
    // Indicate that the other buffer is free now.
    dmaCfg.txIdx[(dmaCfg.txSel ^ 1)] = 0;
    
    // Set TX shadow
    dmaCfg.txShdw = HAL_GetTick();
    dmaCfg.txShdwValid = TRUE;

    // If there is more Tx data ready to go, re-start the DMA immediately on it.
    if (dmaCfg.txIdx[dmaCfg.txSel])
    {
      // UART TX DMA is expected to be fired
      dmaCfg.txDMAPending = TRUE;
    }
  }
}
