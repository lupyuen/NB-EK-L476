//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : gpio.c
//  Date     : 2018-11-02 14:04
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************

/* Includes ------------------------------------------------------------------*/

#include "los_typedef.h"
#include "los_hwi.h"
#include "gpio.h"
#ifdef LOS_STM32L476xx

#include "stm32l4xx_hal.h"
#include "stm32l4xx_nbek.h"

#endif
//-------------------------------------------------------------------------------
// Function prototypes
static HAL_StatusTypeDef GPIO_EnableClock(GPIO_TypeDef* port);
//*******************************************************************************
// fn : Board_GPIO_Init
//
// brief : Init the gpio
//
// param : pinCfg -> the refernce of PIN_Config
//
// return : none
extern void Board_GPIO_Init(const PIN_Config *pinCfg)
{
  if(pinCfg == NULL)
  {
    return;
  }
  
  GPIO_InitTypeDef  GPIO_InitStruct = {0};

  
  if(pinCfg->num)
  {
    for(uint8_t i = 0 ; i < pinCfg->num ; i++)
    {
      Board_GPIO_OneInit(&pinCfg->hwAttrs[i]);
    }
  }
}
//*******************************************************************************
// fn : Board_GPIO_OneInit
//
// brief : Init the gpio
//
// param : pin -> the refernce of PIN_HWAttrs
//
// return : none
extern void Board_GPIO_OneInit(const PIN_HWAttrs *pin)
{
  if(pin == NULL)
  {
    return;
  }
  GPIO_InitTypeDef  GPIO_InitStruct = {0}; 
  GPIO_EnableClock(pin->Port);
  HAL_GPIO_WritePin(pin->Port, pin->pin, pin->state);
  /* Configure the GPIO pin */
  GPIO_InitStruct.Pin   = pin->pin;
  GPIO_InitStruct.Mode  = pin->mode;
  GPIO_InitStruct.Pull  = pin->pull;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  if(pin->mode == GPIO_MODE_AF_PP || pin->mode == GPIO_MODE_AF_OD)
  {
    GPIO_InitStruct.Alternate = pin->alternate;
  }
  HAL_GPIO_Init(pin->Port, &GPIO_InitStruct);
  
  
  //创建并使能中断
  if(GPIO_MODE_IT_RISING_FALLING & pin->mode)
  {
    LOS_HwiCreate(pin->irq, pin->prio,0,pin->irq_func,0);//创建中断
  }
}
//***************************************************************************
// fn : Board_GPIO_DeInit
//
// brief : DeInitialize LED GPIO.
//
// param : 
//
// return : none
//
// ACTION: 
// Board_GPIO_DeInit() does not disable the GPIO clock
void Board_GPIO_DeInit(GPIO_TypeDef* port, uint32_t pin)
{
  /* Turn off LED */
  HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
  /* DeInit the GPIO_LED pin */
  HAL_GPIO_DeInit(port, pin);
}
//***************************************************************************
// fn : Board_GPIO_ReadPin
//
// brief : Return the selected Button state
//
// param : port -> 
//         pin  ->
//
// return : GPIO_PinState

uint32_t Board_GPIO_ReadPin(const PIN_HWAttrs *pin)
{
  return HAL_GPIO_ReadPin(pin->Port, pin->pin);
}
//***************************************************************************
// fn : Board_GPIO_SetPin
//
// brief : Return the selected Button state
//
// param : port -> 
//         pin  ->
//
// return : GPIO_PinState

uint32_t Board_GPIO_SetPin(const PIN_HWAttrs *pin, uint32_t state)
{
  HAL_GPIO_WritePin(pin->Port, pin->pin,state ? GPIO_PIN_SET:GPIO_PIN_RESET);
  return HAL_OK;
}
//**********************************************************************************
// fn : GPIO_EnableClock
//
// brief : Enable the port clock
//
// param : port -> the reference of gpioport
//
// return  : none
static HAL_StatusTypeDef GPIO_EnableClock(GPIO_TypeDef* port)
{
  if(port == NULL)
  {
    return HAL_ERROR;
  }
  
  if(GPIOA == port)
  {
    __HAL_RCC_GPIOA_CLK_ENABLE();
  }
  else if(GPIOB == port)
  {
    __HAL_RCC_GPIOB_CLK_ENABLE();
  }
  else if(GPIOC == port)
  {
    __HAL_RCC_GPIOC_CLK_ENABLE();
  }
#ifdef GPIOD
  else if(GPIOD == port)
  {
    __HAL_RCC_GPIOD_CLK_ENABLE();    
  }
#endif
#ifdef GPIOE
  else if(GPIOE == port)
  {
    __HAL_RCC_GPIOE_CLK_ENABLE();    
  }
#endif
#ifdef GPIOF
  else if(GPIOF == port)
  {
    __HAL_RCC_GPIOF_CLK_ENABLE();    
  }
#endif
#ifdef GPIOG
  else if(GPIOG == port)
  {
    __HAL_RCC_GPIOG_CLK_ENABLE();    
  }
#endif
  else if(GPIOH == port)
  {
    __HAL_RCC_GPIOH_CLK_ENABLE();    
  }
  else
  {
    return HAL_ERROR;
  }
  return HAL_OK;
}
