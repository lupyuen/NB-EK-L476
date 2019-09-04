//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_bsp_gps.c
//  Date     : 2018-11-20 10:10
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#include "los_bsp_gps.h"

#include "stm32l4xx_nbek.h"
#include "usart.h"
#include "hal_uart_isr.h"

#define GPS_PWR_ON     0
#define GPS_PWR_OFF    1

const PIN_HWAttrs gpsPwrPin = 
{
  GPS_PWR_PIN,
  GPS_PWR_PORT,
  GPIO_MODE_OUTPUT_PP,
  GPIO_PULLUP,
  0,
  GPIO_PIN_SET
};

const PIN_HWAttrs gpsTxPin = 
{
  GPSUart_TX_PIN,
  GPSUart_TX_GPIO_PORT,
  GPIO_MODE_AF_PP,
  GPIO_NOPULL,
  GPSUart_AF
};
const PIN_HWAttrs gpsRxPin = 
{
  GPSUart_RX_PIN,
  GPSUart_RX_GPIO_PORT,
  GPIO_MODE_AF_PP,
  GPIO_NOPULL,
  GPSUart_AF
};

const UART_HWAttrs uartGpsHWAttrs = {
  .txPin     = &gpsTxPin,
  .rxPin     = &gpsRxPin
};

UART_HandleTypeDef GPSUartHandle;

const UART_Config gpsUartCfg = {
  .pUart = &GPSUartHandle,
  .hwAttrs = &uartGpsHWAttrs,
  .rxCb = UartIsr_RxCplt,
  .txCb = UartIsr_TxCplt
};


typedef void (*uart_common_cb)(char*, uint16_t);
static uart_common_cb  register_cb = 0;

#ifdef UART_DBG
static uint8_t msgBuf[512];
#endif


//*****************************************************************************
// fn : LOS_EvbGps_Init
//
// brief : Init the gps
//
// param : none
//
// return : none
extern void LOS_EvbGps_Init(void)
{
  GPSUartHandle.Instance = USART2;
  GPSUartHandle.Init.BaudRate = 9600;
  GPSUartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  GPSUartHandle.Init.StopBits = UART_STOPBITS_1;
  GPSUartHandle.Init.Parity = UART_PARITY_NONE;
  GPSUartHandle.Init.Mode = UART_MODE_TX_RX;
  GPSUartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  GPSUartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  GPSUartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  GPSUartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  
  Board_GPIO_OneInit(&gpsPwrPin);
  Board_UART_Init(&gpsUartCfg);
}
//******************************************************************************
// fn : LOS_EvbGpsUartITInit
//
// brief : Init the GPS Uart
//
// param : none
//
// return : none
extern void LOS_EvbGpsUartITInit(void*cb)
{
  uint8_t * buf = UartIsr_GetBuf();
  register_cb = (uart_common_cb)cb;
  UartIsr_Init(&GPSUartHandle);
  Board_GPIO_SetPin(&gpsPwrPin,GPS_PWR_ON);
  Board_UART_Rece(&gpsUartCfg,buf,RECE_BUF_MAX_LEN);
}

//******************************************************************************
// fn : LOS_EvbGpsUartWrite
//
// brief : 通过串口向外发送指定数据
//
// param : buf -> 数据缓存地址
//         len -> 数据长度
//
// return : none
void LOS_EvbGpsUartWrite(uint8_t *buf,uint16_t len)
{
#ifdef UART_DBG
  buf[len] = 0;
  printf("\r\n->:%s",buf);
#endif
  Board_UART_Send(&gpsUartCfg,buf,len);
  
  //UartIsr_Write(buf,len);
}
//******************************************************************************
// fn : LOS_EvbGpsUartClose
//
// brief : Close the uart peripheral
//
// param : none
//
// return : none
extern void LOS_EvbGpsUartClose(void)
{
	Board_UART_DeInit(&gpsUartCfg);
  Board_GPIO_SetPin(&gpsPwrPin,GPS_PWR_OFF);
}
//******************************************************************************
// fn : LOS_EvbGpsUartPoll
//
// brief : 轮询串口事件
//
// param : none
//
// return : 事件
uint8_t LOS_EvbGpsUartPoll(void)
{
  uint8_t evt =  UartIsr_Poll();
  uint16_t len = 0;
  if(evt)
  {
    len = UartIsr_Avail();
    if(len)
    {
      len = UartIsr_Read(msgBuf,len);
#ifdef UART_DBG
      msgBuf[len] = 0;
      printf("\r\n<-:%s",msgBuf);
#endif
      if(register_cb)
      {
        register_cb((char*)msgBuf,len);
      }
    }
    
  }
  return evt;  
  
}

