//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : lptim.c
//  Date     : 2018-11-15 22:10
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#include "lptim.h"
extern LPTIM_Config lightCfg;
//*************************************************************************
// fn : Board_LPTIM_Init
//
// brief : Init the peripheral
//
// param : cfg -> the reference of LPTIM_Config obj
//
// return : HAL_ERROR or HAL_OK
extern uint32_t Board_LPTIM_Init(const LPTIM_Config* cfg)
{
	if(cfg == NULL)
	{
		return HAL_ERROR;
	}
	
  if (HAL_LPTIM_Init(cfg->phwattrs->hlptim1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	
	return HAL_OK;
}
//**************************************************************
// fn : HAL_LPTIM_MspInit
//
// brief: 系统调用，开发者不可调用
//
// param : lptimHandle -> the reference
//
// return : none
void HAL_LPTIM_MspInit(LPTIM_HandleTypeDef* lptimHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(lptimHandle->Instance==LPTIM1)
  {
    /* LPTIM1 clock enable */
    __HAL_RCC_LPTIM1_CLK_ENABLE();
  
    /**LPTIM1 GPIO Configuration    
    PB2     ------> LPTIM1_OUT 
    */
//    GPIO_InitStruct.Pin = GPIO_PIN_2;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF1_LPTIM1;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    Board_GPIO_OneInit(lightCfg.phwattrs->pTimpin);
  }
}
//**************************************************************
// fn : HAL_LPTIM_MspDeInit
//
// brief: 系统调用，开发者不可调用
//
// param : lptimHandle -> the reference
//
// return : none
void HAL_LPTIM_MspDeInit(LPTIM_HandleTypeDef* lptimHandle)
{

  if(lptimHandle->Instance==LPTIM1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_LPTIM1_CLK_DISABLE();
  
    /**LPTIM1 GPIO Configuration    
    PB2     ------> LPTIM1_OUT 
    */
    //HAL_GPIO_DeInit(GPIOB, GPIO_PIN_2);
    Board_GPIO_DeInit(lightCfg.phwattrs->pTimpin->Port,lightCfg.phwattrs->pTimpin->pin);
  }
}
//*************************************************************************
// fn : Board_LPTIM_Close
//
// brief : Init the peripheral
//
// param : cfg -> the reference of LPTIM_Config obj
//
// return : HAL_ERROR or HAL_OK
extern uint32_t Board_LPTIM_Close(const LPTIM_Config* cfg)
{
	return HAL_LPTIM_DeInit(cfg->phwattrs->hlptim1);
}

//*************************************************************************
// fn : Board_LPTIM_PWM
//
// brief : Init the peripheral
//
// param : cfg -> the reference of LPTIM_Config obj
//         duty->
//
// return : HAL_ERROR or HAL_OK
extern uint32_t Board_LPTIM_PWM(const LPTIM_Config* cfg,uint8_t duty)
{
	if(cfg == NULL)
	{
		return HAL_ERROR;
	}
	if(duty > 100)
  {
    duty = 100;
  }
  duty = 100 - duty;
  if(duty == 100)
  {
		Board_LPTIM_Close(cfg);
    return HAL_OK;
  }
  else
  {
    Board_LPTIM_Init(cfg);
  }
  HAL_LPTIM_PWM_Start(cfg->phwattrs->hlptim1, 100,duty);
	return HAL_OK;
}
