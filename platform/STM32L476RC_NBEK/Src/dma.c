//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : dma.c
//  Date     : 2018-11-02 14:04
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
/* Includes ------------------------------------------------------------------*/
#include "dma.h"
#include "los_hwi.h"

#ifdef LOS_STM32L476xx
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nbek.h"
#include "stm32l4xx_it.h"
#endif

DMA_HandleTypeDef hdma_lpuart_rx;
DMA_HandleTypeDef hdma_lpuart_tx;

//*******************************************************************
// 定义lpuart Rx DMA接收通道
//
DMA_Config lpuartRxDmaCfg = 
{
  .dmaHandle =  &hdma_lpuart_rx,
  .irq       =  DMA2_Channel7_IRQn,
  .irq_func  =  DMA2_Channel7_IRQHandler,
  .prio      =  6
};
//********************************************************************
// 定义lpuart Tx DMA接收通道
//
DMA_Config lpuartTxDmaCfg = 
{
  .dmaHandle =  &hdma_lpuart_tx,
  .irq       =  DMA2_Channel6_IRQn,
  .irq_func  =  DMA2_Channel6_IRQHandler,
  .prio      =  6
};

//------------------------------------------------------------------------------
// static function prototypes 
/* trun on/off the dma clock*/
static uint32_t DMA_Clk_OnOff(DMA_Config* dmaCfg,uint8_t opt);

//******************************************************************************
// fn : Board_DMA_Init
//
// brief : Enable DMA controller clock
//
// param : dmaCfg -> the reference of the DMA Configuration
//
// return : none
uint32_t Board_DMA_Init(DMA_Config* dmaCfg)
{
  if(dmaCfg == NULL)
  {
    return HAL_ERROR;
  }
  
  /* DMA controller clock enable */
  DMA_Clk_OnOff(dmaCfg,TRUE);

  /* DMA interrupt init */
  
  /* DMA2_Channel6_IRQn interrupt configuration */
  //HAL_NVIC_SetPriority(DMA2_Channel6_IRQn, 6, 0);
  //HAL_NVIC_EnableIRQ(DMA2_Channel6_IRQn);
  /* DMA2_Channel7_IRQn interrupt configuration */
  //HAL_NVIC_SetPriority(DMA2_Channel7_IRQn, 6, 0);
  //HAL_NVIC_EnableIRQ(DMA2_Channel7_IRQn);
  
  if (HAL_DMA_Init(dmaCfg->dmaHandle) != HAL_OK)
  {
    return HAL_ERROR;
  }
  
  LOS_HwiCreate(dmaCfg->irq, dmaCfg->prio,0,dmaCfg->irq_func,0);
  return HAL_OK;
}
//******************************************************************************
// fn : Board_DMA_DeInit
//
// brief : Disable DMA controller clock
//
// param : dmaCfg -> the reference of the DMA Configuration
//
// return : none
extern uint32_t Board_DMA_DeInit(DMA_Config* dmaCfg)
{
  if(dmaCfg == NULL)
  {
    return HAL_ERROR;
  }
  
  return HAL_DMA_DeInit(dmaCfg->dmaHandle);
}
//*******************************************************************************
// fn : DMA_Clk_OnOff
//
// brief : Enable/Disable the dma peripheral clock
//
// param : dmaCfg -> the reference of the DMA_Config
//         opt    -> true -> enable; false -> disable
//
static uint32_t DMA_Clk_OnOff(DMA_Config* dmaCfg, uint8_t opt)
{
  if ((uint32_t)(dmaCfg->dmaHandle->Instance) < (uint32_t)(DMA2_Channel1))
  {
    /* DMA1 */
    if(opt)
    {
      __HAL_RCC_DMA1_CLK_ENABLE();
    }
    else
    {
      __HAL_RCC_DMA1_CLK_DISABLE();
    }

  }
  else
  {
    /* DMA2 */
    if(opt)
    {
      __HAL_RCC_DMA2_CLK_ENABLE();
    }
    else
    {
      __HAL_RCC_DMA2_CLK_DISABLE();      
    }
  }
  return HAL_OK;
}

