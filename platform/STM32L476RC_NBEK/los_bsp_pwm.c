//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_bsp_pwm.c
//  Date     : 2018-11-15 22:36
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#include "los_bsp_pwm.h"

#include "stm32l4xx_nbek.h"
#include "lptim.h"
#include "gpio.h"

LPTIM_HandleTypeDef  LightTim;

const PIN_HWAttrs  lptimPin = 
{
	LIGHT_TIM_GPIO_PIN,
	LIGHT_TIM_GPIO_PORT,
	GPIO_MODE_AF_PP,
	GPIO_NOPULL,
	GPIO_AF1_LPTIM1
};
const LPTIM_HWAttrs lightHWAtts = 
{
	.pTimpin = &lptimPin,
	.hlptim1 = &LightTim
};

LPTIM_Config lightCfg = 
{
	.phwattrs = &lightHWAtts
};

//*********************************************************************************
// fn : LOS_EvbPwm_Init
//
// brief : Init the Light
//
// param : none
//
// return : none
extern void LOS_EvbPwm_Init(void)
{
	LightTim.Instance             = LPTIM1;
  LightTim.Init.Clock.Source    = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  LightTim.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
  LightTim.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
  LightTim.Init.OutputPolarity = LPTIM_OUTPUTPOLARITY_HIGH;
  LightTim.Init.UpdateMode     = LPTIM_UPDATE_IMMEDIATE;
  LightTim.Init.CounterSource  = LPTIM_COUNTERSOURCE_INTERNAL;
  LightTim.Init.Input1Source   = LPTIM_INPUT1SOURCE_GPIO;
  LightTim.Init.Input2Source   = LPTIM_INPUT2SOURCE_GPIO;
	
	Board_LPTIM_Init(&lightCfg);
}
//*********************************************************************************
// fn : LOS_EvbPwm_Set
//
// brief : Set the duty
//
// param : none
//
// return : none
extern void LOS_EvbPwm_Set(uint8_t duty)
{
	Board_LPTIM_PWM(&lightCfg,duty);
}
