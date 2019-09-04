//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_bsp_lcd.c
//  Date     : 2018-11-01 20:57
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nbek.h"

typedef struct
{
  uint32_t         pin;
  GPIO_TypeDef*    Port;
  uint32_t         mode;
  uint32_t         pull;
  uint32_t         alternate;
  GPIO_PinState    state;
  
  IRQn_Type        irq;      //只有MODE是GPIO_MODE_IT_RISING || GPIO_MODE_IT_FALLING || GPIO_MODE_IT_RISING_FALLING
  IrqFuncPtr       irq_func;
  uint8_t          prio;
}PIN_HWAttrs;

typedef struct
{
  uint8_t            num;    //记录PIN_HWAttrs的元素个数
  PIN_HWAttrs const *hwAttrs;
}PIN_Config;
//*******************************************************************************
// fn : Board_GPIO_Init
//
// brief : Init the gpio
//
// param : pinCfg -> the refernce of PIN_Config
//
// return : none
extern void Board_GPIO_Init(const PIN_Config *pinCfg);
//*******************************************************************************
// fn : Board_GPIO_OneInit
//
// brief : Init the gpio
//
// param : pin -> the refernce of PIN_HWAttrs
//
// return : none
extern void Board_GPIO_OneInit(const PIN_HWAttrs *pin);

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
extern void Board_GPIO_DeInit(GPIO_TypeDef* port, uint32_t pin);
//***************************************************************************
// fn : Board_GPIO_ReadPin
//
// brief : Return the selected Button state
//
// param : port -> 
//         pin  ->
//
// return : GPIO_PinState
extern uint32_t Board_GPIO_ReadPin(const PIN_HWAttrs *pin);

//***************************************************************************
// fn : Board_GPIO_SetPin
//
// brief : Return the selected Button state
//
// param : port -> 
//         pin  ->
//
// return : GPIO_PinState
uint32_t Board_GPIO_SetPin(const PIN_HWAttrs *pin, uint32_t state);


#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

