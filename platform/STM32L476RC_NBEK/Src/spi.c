//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : spi.c
//  Date     : 2018-11-05 11:15
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#include "spi.h"
#ifdef LOS_STM32L476xx

#include "stm32l4xx_hal.h"
#include "stm32l4xx_nbek.h"

#endif
extern const SPI_Config  lcdSpiCfg;

//-------------------------------------------------------------------------------
// static function prototype
//
/* turn on/off the spi clk */
static HAL_StatusTypeDef SPI_Clk_OnOff(SPI_TypeDef* spiBase, uint8_t opt);
//*******************************************************************************
// fn : Board_SPI_Init
//
// brief : Init the spi perpheral
//
// param : spiCfg -> the spi configuration
//
// return : success or fail
extern uint32_t Board_SPI_Init(const SPI_Config* spiCfg)
{
  if(spiCfg == NULL)
  {
    return HAL_ERROR;
  }

  if (HAL_SPI_Init(spiCfg->spiHandle) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  return HAL_OK;
}
//******************************************************************************
// fn : HAL_SPI_MspInit
//
// brief : Init the spi perpheral
//
// param : spiHandle -> the reference of the spi perpheral
//
// return : none
// ACTION:
// 此函数开发者不要自行调用，它由Board_SPI_Init调用
void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(spiHandle->Instance == lcdSpiCfg.spiHandle->Instance)
  {
    /* SPI1 clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();
  
    /**SPI1 GPIO Configuration    
    PA5     ------> SPI1_SCK
    PA7     ------> SPI1_MOSI 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

//****************************************************************************
// fn : Board_SPI_DeInit
//
// brief : Deinit the spi perpheral
//
// param : spiCfg -> the spi configuration
//
// return : success or fail
extern uint32_t Board_SPI_DeInit(const SPI_Config* spiCfg)
{
  if(spiCfg == NULL)
  {
    return HAL_ERROR;
  }

  if (HAL_SPI_DeInit(spiCfg->spiHandle) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  return HAL_OK;
  
}
//******************************************************************************
// fn : HAL_SPI_MspDeInit
//
// brief : Deinit the spi perpheral
//
// param : spiHandle -> the reference of the spi perpheral
//
// return : none
// ACTION:
// 此函数开发者不要自行调用，它由Board_SPI_Deinit自行调用
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance == lcdSpiCfg.spiHandle->Instance)
  {
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();

    /**SPI1 GPIO Configuration    
    PA5     ------> SPI1_SCK
    PA7     ------> SPI1_MOSI 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5|GPIO_PIN_7);

  }
}
//**********************************************************************************
// fn : SPI_Clk_OnOff
//
// brief : turn on/off the spi clock
//
// param : spiBase -> the pointer of spi perphral
//         opt    -> ture : enable ; false -> disable
//
// return : none
static HAL_StatusTypeDef SPI_Clk_OnOff(SPI_TypeDef* spiBase, uint8_t opt)
{
  if(spiBase == NULL)
  {
    return HAL_ERROR;
  }
  
  if(spiBase == SPI1)
  {
    if(opt)
    {
      __HAL_RCC_SPI1_CLK_ENABLE();
    }
    else 
    {
      __HAL_RCC_SPI1_CLK_DISABLE();
    }
  }
  else if(spiBase == SPI2)
  {
    if(opt)
    {
      __HAL_RCC_SPI2_CLK_ENABLE();
    }
    else 
    {
      __HAL_RCC_SPI2_CLK_DISABLE();
    }
  }
  else if(spiBase == SPI3)
  {
    if(opt)
    {
      __HAL_RCC_SPI3_CLK_ENABLE();
    }
    else 
    {
      __HAL_RCC_SPI3_CLK_DISABLE();
    }
  }
  else
  {
    return HAL_ERROR;
  }
  
  return HAL_OK;
}
