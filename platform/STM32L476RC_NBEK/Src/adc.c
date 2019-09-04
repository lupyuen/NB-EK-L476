//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : adc.c
//  Date     : 2018-11-15 10:36
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#include "adc.h"
#include "los_mux.h"
#include "los_hwi.h"

#ifdef LOS_STM32L476xx
#include "stm32l4xx_nbek.h"
#include "stm32l4xx_it.h"
#include "gpio.h"
#endif

extern ADC_Config  luxAdcCfg;
//********************************************************************************
// fn : Board_ADC_Init
//
// brief : Init the adc periphral
//
// param : adcCfg -> the reference of ADC_Config
//
// return : HAL_OK or HAL_ERROR
extern uint32_t Board_ADC_Init(const ADC_Config* adcCfg)
{
  if(adcCfg ==  NULL)
  {
    return HAL_ERROR;
  }
  
  __HAL_RCC_ADC_FORCE_RESET();
  
  __HAL_RCC_ADC_RELEASE_RESET();  
  if (HAL_ADC_Init(adcCfg->phwattrs->pAdc) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  
  if (HAL_ADCEx_MultiModeConfigChannel(adcCfg->phwattrs->pAdc, (ADC_MultiModeTypeDef*)adcCfg->pMode) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  
  if (HAL_ADC_ConfigChannel(adcCfg->phwattrs->pAdc, (ADC_ChannelConfTypeDef*)adcCfg->pChCfg) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  return HAL_OK;
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{
  if(adcHandle->Instance == ADC1)
  {
    /* ADC1 clock enable */
    __HAL_RCC_ADC_CLK_ENABLE();
  
    /**ADC1 GPIO Configuration    
    PB1     ------> ADC1_IN16 
    */
    Board_GPIO_OneInit(luxAdcCfg.phwattrs->adcPin);

  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance == ADC1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_ADC_CLK_DISABLE();
  
    /**ADC1 GPIO Configuration    
    PB1     ------> ADC1_IN16 
    */
    Board_GPIO_DeInit(luxAdcCfg.phwattrs->adcPin->Port,luxAdcCfg.phwattrs->adcPin->pin);
  }
}

//********************************************************************************
// fn : Board_ADC_Read
//
// brief : Read a value from adc
//
// param : adcCfg -> the reference of ADC_Config
//
// return : value
extern uint16_t Board_ADC_Read(const ADC_Config* adcCfg)
{
  uint16_t ad_value = 0;
  if(adcCfg == NULL)
  {
    return ad_value;
  }
  LOS_MuxPend(luxAdcCfg.pOjb->muxId, LOS_WAIT_FOREVER);
  
  HAL_ADC_Start(adcCfg->phwattrs->pAdc);

  //Wait for end of connversion
  HAL_ADC_PollForConversion(adcCfg->phwattrs->pAdc, luxAdcCfg.pOjb->timeout);

  /* Check if the continous conversion of regular channel is finished */  
  if(HAL_IS_BIT_SET(HAL_ADC_GetState(adcCfg->phwattrs->pAdc), HAL_ADC_STATE_REG_EOC)) 
  {
    /*##-3- Get the converted value of regular channel  ######################*/
    ad_value = HAL_ADC_GetValue(adcCfg->phwattrs->pAdc);
  }
  LOS_MuxPost(luxAdcCfg.pOjb->muxId);
  return ad_value; 
  
}
//********************************************************************************
// fn : Board_ADC_Close
//
// brief : Close a value from adc
//
// param : adcCfg -> the reference of ADC_Config
//
// return : value
extern uint16_t Board_ADC_Close(const ADC_Config* adcCfg)
{
  HAL_ADC_DeInit(adcCfg->phwattrs->pAdc);
}
