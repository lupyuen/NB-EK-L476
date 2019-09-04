//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_bsp_nbuart.c
//  Date     : 2018-10-28 14:06
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#include <string.h>
#include "los_bsp_nbuart.h"
#include "los_bsp_nben.h"

#ifdef LOS_STM32L476xx

#include "stm32l4xx_nbek.h"

#include "usart.h"
#include "NB_BC95.h"
#include "_dma_cfg.h"


//配置串口硬件相关信息

PIN_HWAttrs  nbTxPin = {
  
  NBUart_TX_PIN,
  NBUart_TX_GPIO_PORT,
  GPIO_MODE_AF_PP,
  GPIO_PULLUP,
  NBUart_AF,
};

PIN_HWAttrs nbRxPin = {
  NBUart_RX_PIN,
  NBUart_RX_GPIO_PORT,
  GPIO_MODE_AF_PP,
  GPIO_NOPULL,
  NBUart_AF
};
const UART_HWAttrs uartNbHWAttrs = {
  .txPin = &nbTxPin,
  .rxPin = &nbRxPin
};

UART_HandleTypeDef NbUartHandle;

const UART_Config nbUartCfg = {
  .pUart = &NbUartHandle,
  .hwAttrs = &uartNbHWAttrs,
  .rxCb = NULL,
  .txCb = UartDma_TxCpltCB
};


static bc95_receive_cb  nb_receCb = NULL;
static uint8_t msgBuf[256];

#endif
//--------------------------------------------------------------------------------
// the static function prototypes
//

/* brief : 通过串口向外发送指定数据 */
static void uart_dma_send(uint8_t* buf,uint16_t len);
//--------------------------------------------------------------------------------
//******************************************************************************
// fn : LOS_EvbNBUartInit
//
// brief : Init the NBUart
//
// param : none
//
// return : none
extern void LOS_EvbNBUartInit(void)
{
#ifdef LOS_STM32L476xx
  LOS_EvbNBPwrInit();
  
  
  NbUartHandle.Instance = NBUart;
  NbUartHandle.Init.BaudRate = 9600;
  NbUartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  NbUartHandle.Init.StopBits = UART_STOPBITS_1;
  NbUartHandle.Init.Parity = UART_PARITY_NONE;
  NbUartHandle.Init.Mode = UART_MODE_TX_RX;
  NbUartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  NbUartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  NbUartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  
  Board_UART_Init(&nbUartCfg);
#endif
}
//******************************************************************************
// fn : LOS_EvbNBUartDMAInit
//
// brief : Init the NBUart
//
// param : none
//
// return : none
extern void LOS_EvbNBUartDMAInit(bc95_receive_cb cb,uint32_t baud)
{
  baud = baud;
  nb_receCb = cb;
  uint8_t* buf = UartDma_Init(uart_dma_send,NBUart);

  //nbUartCfg.rxBuf = buf;
  Board_UART_Rece(&nbUartCfg,buf,RECE_BUF_MAX_LEN);
}
//******************************************************************************
// fn : LOS_EvbNBUartWrite
//
// brief : 通过串口向外发送指定数据
//
// param : buf -> 数据缓存地址
//         len -> 数据长度
//
// return : none
void LOS_EvbNBUartWrite(uint8_t *buf,uint16_t len)
{
#ifdef UART_DBG
  buf[len] = 0;
  printf("\r\n->:%s",buf);
#endif
  UartDma_Write(buf,len);
}
//******************************************************************************
// fn : LOS_EvbNBUartClose
//
// brief : Close the uart peripheral
//
// param : none
//
// return : none
extern void LOS_EvbNBUartClose(void)
{
	Board_UART_DeInit(&nbUartCfg);
}
//******************************************************************************
// fn : LOS_EvbNBUartPoll
//
// brief : 轮询串口事件
//
// param : none
//
// return : 事件
uint8_t LOS_EvbNBUartPoll(void)
{
  uint8_t evt =  UartDma_Poll();
  uint16_t len = 0;
  if(evt)
  {
    len = UartDma_Avail();
    if(len)
    {
      len = UartDma_Read(msgBuf,len);
#ifdef UART_DBG
      msgBuf[len] = 0;
      printf("\r\n<-:%s",msgBuf);
#endif
      if(nb_receCb)
      {
        nb_receCb((char*)msgBuf,len);
      }
    }
    
  }
  return evt;  
  
}
//******************************************************************************
// fn : uart_dma_send
//
// brief : 通过串口向外发送指定数据
//
// param : buf -> 数据缓存地址
//         len -> 数据长度
//
// return : none
static void uart_dma_send(uint8_t* buf,uint16_t len)
{
  Board_UART_Send(&nbUartCfg,buf,len);
}
