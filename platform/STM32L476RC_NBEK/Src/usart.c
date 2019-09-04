//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : usart.c
//  Date     : 2018-11-02 14:04
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************

/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include "los_bsp_uart.h"
#include "los_hwi.h"

#ifdef LOS_STM32L476xx
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nbek.h"
#include "stm32l4xx_it.h"
#include "dma.h"
#endif

extern DMA_HandleTypeDef hdma_lpuart_rx;
extern DMA_HandleTypeDef hdma_lpuart_tx;
extern DMA_Config lpuartRxDmaCfg;
extern DMA_Config lpuartTxDmaCfg;

extern const UART_Config nbUartCfg;
extern const UART_Config gpsUartCfg;
//*****************************************************************************
// fn : Board_UART_Init
//
// brief : Init the uart
//
// param : none
//
// return : none
void Board_UART_Init(const UART_Config *uartCfg )
{
  if (HAL_UART_Init(uartCfg->pUart) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}
//*****************************************************************************
// fn : Board_UART_DeInit
//
// brief : Deinit the uart
//
// param : uartCfg -> pointer of UART_Config
//
// return : none
extern void Board_UART_DeInit(const UART_Config *uartCfg)
{
	if (uartCfg->pUart == NULL )
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  HAL_UART_DeInit(uartCfg->pUart);
}

//*****************************************************************************
// fn : Board_UART_Send
//
// brief : Send the data by uart
//
// param : uartCfg -> pointer of UART_Config
//
// return : none
extern void Board_UART_Send(const UART_Config *uartCfg,uint8_t *buf, uint16_t len)
{
  if (uartCfg->pUart == NULL )
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  
  if(uartCfg->pUart->Instance == NBUart)
  {
    HAL_UART_Transmit_DMA(uartCfg->pUart,buf,len); 
  }
  else if(uartCfg->pUart->Instance == GPSUart)
  {
    HAL_UART_Transmit_IT(uartCfg->pUart,buf,len);    
  }  
  
}
//*****************************************************************************
// fn : Board_UART_Rece
//
// brief : Rece the data by uart
//
// param : uartCfg -> pointer of UART_Config
//
// return : none
extern void Board_UART_Rece(const UART_Config *uartCfg,uint8_t *buf, uint16_t len)
{
  if (uartCfg->pUart == NULL )
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  
  if(uartCfg->pUart->Instance == NBUart)
  {
    HAL_UART_Receive_DMA(uartCfg->pUart,buf,len); 
  }
  else if(uartCfg->pUart->Instance == GPSUart)
  {
    HAL_UART_Receive_IT(uartCfg->pUart,buf,len);    
  }
  
}
//*****************************************************************************
// fn : HAL_UART_MspInit
//
// brief : cfg the gpio in uart
//
// param : uartHandle -> the handle of uart module
//
// return : none
void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(uartHandle->Instance == LOGx)
  {
    /* USART1 clock enable */
    LOGx_CLK_ENABLE();
    LOGx_TXRX_GPIO_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10    ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = LOGx_TX_PIN|LOGx_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = LOGx_AF;
    HAL_GPIO_Init(LOGx_GPIO_PORT, &GPIO_InitStruct);
  }
  else if(uartHandle->Instance == NBUart)
  {
    /* LPUart1 clock enable */
    NBUart_CLK_ENABLE();
    NBUart_TXRX_GPIO_CLK_ENABLE();   
    
    /**LPUART1 GPIO Configuration    
    PB10     ------> LPUART1_RX
    PB11     ------> LPUART1_TX 
    */
    GPIO_InitStruct.Pin = NBUart_TX_PIN|NBUart_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = NBUart_AF;
    HAL_GPIO_Init(NBUart_GPIO_PORT, &GPIO_InitStruct);

    /* LPUART1 DMA Init */
    /* LPUART_RX Init */
    lpuartRxDmaCfg.dmaHandle->Instance = DMA2_Channel7;
    lpuartRxDmaCfg.dmaHandle->Init.Request = DMA_REQUEST_4;
    lpuartRxDmaCfg.dmaHandle->Init.Direction = DMA_PERIPH_TO_MEMORY;
    lpuartRxDmaCfg.dmaHandle->Init.PeriphInc = DMA_PINC_DISABLE;
    lpuartRxDmaCfg.dmaHandle->Init.MemInc = DMA_MINC_ENABLE;
    lpuartRxDmaCfg.dmaHandle->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    lpuartRxDmaCfg.dmaHandle->Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    lpuartRxDmaCfg.dmaHandle->Init.Mode = DMA_CIRCULAR;
    lpuartRxDmaCfg.dmaHandle->Init.Priority = DMA_PRIORITY_LOW;
    
    Board_DMA_Init(&lpuartRxDmaCfg);
    
    __HAL_LINKDMA(uartHandle,hdmarx,hdma_lpuart_rx);
    
    /* LPUART_TX Init */
    lpuartTxDmaCfg.dmaHandle->Instance = DMA2_Channel6;
    lpuartTxDmaCfg.dmaHandle->Init.Request = DMA_REQUEST_4;
    lpuartTxDmaCfg.dmaHandle->Init.Direction = DMA_MEMORY_TO_PERIPH;
    lpuartTxDmaCfg.dmaHandle->Init.PeriphInc = DMA_PINC_DISABLE;
    lpuartTxDmaCfg.dmaHandle->Init.MemInc = DMA_MINC_ENABLE;
    lpuartTxDmaCfg.dmaHandle->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    lpuartTxDmaCfg.dmaHandle->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    lpuartTxDmaCfg.dmaHandle->Init.Mode = DMA_NORMAL;
    lpuartTxDmaCfg.dmaHandle->Init.Priority = DMA_PRIORITY_LOW;
    Board_DMA_Init(&lpuartTxDmaCfg);

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_lpuart_tx);

    /* LPUART1 interrupt Init */
    /* Create the LPUART interrupte*/
    LOS_HwiCreate(LPUART1_IRQn, 6,0,LPUART1_IRQHandler,0);
  }
  else if(uartHandle->Instance == GPSUart)
  {
    /* Uart2 clock enable */
    GPSUart_CLK_ENABLE();
    
    /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX 
    */    
    Board_GPIO_OneInit(gpsUartCfg.hwAttrs->txPin);
    Board_GPIO_OneInit(gpsUartCfg.hwAttrs->rxPin);
    /* UART2 interrupt Init */
    /* Create the UART interrupte*/    
    LOS_HwiCreate(USART2_IRQn, 6,0,USART2_IRQHandler,0);
  }
  
}
//*****************************************************************************
// fn : HAL_UART_MspDeInit
//
// brief : DeInit the uart
//
// param : uartHandle -> the handle of uart module
//
// return : none
void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{
  if(uartHandle->Instance == LOGx)
  {

    /* Peripheral clock disable */
    LOGx_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(LOGx_GPIO_PORT, LOGx_TX_PIN|LOGx_RX_PIN);
  }
  else if(uartHandle->Instance == NBUart)
  {
    /* Peripheral clock disable */
    __HAL_RCC_LPUART1_CLK_DISABLE();
  
    /**LPUART1 GPIO Configuration    
    PB10     ------> LPUART1_RX
    PB11     ------> LPUART1_TX 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

    /* LPUART1 DMA DeInit */
    //Board_DMA_DeInit(&lpuartTxDmaCfg);
    //Board_DMA_DeInit(&lpuartRxDmaCfg);
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* LPUART1 interrupt Deinit */
    LOS_HwiDelete(LPUART1_IRQn);
  }
}

//***********************************************************************
// fn : HAL_UART_ErrorCallback
//
// brief : 串口接收回调函数，由系统自行调用
//
// param : huart -> 串口实例引用
//
// return : none
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == NBUart)
  {
    HAL_UART_Receive_DMA(huart,huart->pRxBuffPtr,huart->RxXferSize); 
  }
  
}
//**************************************************************************************
// fn : HAL_UART_TxCpltCallback
//
// brief : 串口中断回调函数
//
// param : huart -> the reference of uart 
//
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == NBUart)
  {
    if(nbUartCfg.txCb)
    {
      nbUartCfg.txCb(huart);
    }
  }
  else if(huart->Instance == GPSUart)
  {
    if(gpsUartCfg.txCb)
    {
      gpsUartCfg.txCb(huart);
    }
  }
    
}
//**************************************************************************************
// fn : HAL_UART_RxCpltCallback
//
// brief : 串口中断回调函数
//
// param : huart -> the reference of uart 
//
// return : none
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == GPSUart)
  {
    gpsUartCfg.rxCb(huart);
  }

}
