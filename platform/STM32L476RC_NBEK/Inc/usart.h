
#ifndef __usart_H
#define __usart_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "gpio.h"

typedef void (*Uart_rxCb)(UART_HandleTypeDef* handle);  //接收完成回调
typedef void (*Uart_txCb)(UART_HandleTypeDef* handle);  //发送完成回调

typedef struct
{
//  uint32_t      txPin;
//  GPIO_TypeDef* txPinPort;
//  
//  uint32_t      rxPin;
//  GPIO_TypeDef* rxPinPort;
//  uint8_t       uartAf;
  const PIN_HWAttrs* txPin;
  const PIN_HWAttrs* rxPin;
}UART_HWAttrs;

typedef struct
{
  UART_HandleTypeDef  *pUart;
  UART_HWAttrs const  *hwAttrs;
  Uart_rxCb            rxCb;
  Uart_txCb            txCb;
}UART_Config,*pUartCfg;

//*****************************************************************************
// fn : Board_UART_Init
//
// brief : Init the uart
//
// param : uartCfg -> pointer of UART_Config
//
// return : none
extern void Board_UART_Init(const UART_Config *uartCfg);

//*****************************************************************************
// fn : Board_UART_DeInit
//
// brief : Deinit the uart
//
// param : uartCfg -> pointer of UART_Config
//
// return : none
extern void Board_UART_DeInit(const UART_Config *uartCfg);

//*****************************************************************************
// fn : Board_UART_Send
//
// brief : Send the data by uart
//
// param : uartCfg -> pointer of UART_Config
//
// return : none
extern void Board_UART_Send(const UART_Config *uartCfg,uint8_t *buf, uint16_t len);

//*****************************************************************************
// fn : Board_UART_Rece
//
// brief : Rece the data by uart
//
// param : uartCfg -> pointer of UART_Config
//
// return : none
extern void Board_UART_Rece(const UART_Config *uartCfg,uint8_t *buf, uint16_t len);
#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */


