//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_bsp_lux.c
//  Date     : 2018-11-15 11:37
//  Version  : V0001
//  History Record : The first
//  Description : 
//******************************************************************************
#include "los_bsp_lux.h"
#include "los_mux.h"

#ifdef LOS_STM32L476xx

#include "stm32l4xx_nbek.h"
#include "adc.h"

#endif

ADC_HandleTypeDef             hLuxAdc;
const ADC_MultiModeTypeDef    luxMultiMode = 
{
  .Mode = ADC_MODE_INDEPENDENT
};

const ADC_ChannelConfTypeDef  luxConfig = {
  .Channel = ADC_CHANNEL_16,
  .Rank = ADC_REGULAR_RANK_1,
  .SamplingTime = ADC_SAMPLETIME_2CYCLES_5,
  .SingleDiff = ADC_SINGLE_ENDED,
  .OffsetNumber = ADC_OFFSET_NONE,
  .Offset = 0
};

const PIN_HWAttrs luxPin = {
  LUX_ADCx_GPIO_PIN,                //pin
  LUX_ADCx_GPIO_PORT,               //port
  GPIO_MODE_ANALOG_ADC_CONTROL,     //mode
  GPIO_NOPULL                       //pull
};

const ADC_HWAttrs  luxHWAttrs = {
  .adcPin = &luxPin,
  .pAdc   = &hLuxAdc
};

ADC_Object  luxObj = {
  0,
  10,
  ADC_MODE_BLOCK
};

ADC_Config  luxAdcCfg = {
  .phwattrs = &luxHWAttrs,
  .pMode    = &luxMultiMode,
  .pChCfg   = &luxConfig,
  .pOjb     = &luxObj
  
};

//******************************************************************************
// fn : LOS_EvbLux_Init
//
// brief : init the adc
//
// param : none
//
// return : none
extern void LOS_EvbLux_Init(void)
{
  hLuxAdc.Instance            = LUX_ADCx;
  hLuxAdc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hLuxAdc.Init.Resolution     = ADC_RESOLUTION_12B;
  hLuxAdc.Init.DataAlign      = ADC_DATAALIGN_RIGHT;
  hLuxAdc.Init.ScanConvMode   = ADC_SCAN_DISABLE;
  hLuxAdc.Init.EOCSelection   = ADC_EOC_SINGLE_CONV;
  hLuxAdc.Init.LowPowerAutoWait   = DISABLE;
  hLuxAdc.Init.ContinuousConvMode = DISABLE;
  hLuxAdc.Init.NbrOfConversion    = 1;
  hLuxAdc.Init.DiscontinuousConvMode = DISABLE;
  hLuxAdc.Init.NbrOfDiscConversion   = 1;
  hLuxAdc.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
  hLuxAdc.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hLuxAdc.Init.DMAContinuousRequests = DISABLE;
  hLuxAdc.Init.Overrun               = ADC_OVR_DATA_PRESERVED;
  hLuxAdc.Init.OversamplingMode      = DISABLE;
  
  LOS_MuxCreate(&luxObj.muxId);
  
  Board_ADC_Init(&luxAdcCfg);
}
//******************************************************************************
// fn : LOS_EvbLux_Read
//
// brief : Read lux 
//
// param : none
//
// return : none
extern uint16_t LOS_EvbLux_Read(void)
{
  return Board_ADC_Read(&luxAdcCfg);
}

//******************************************************************************
// fn : LOS_EvbLux_Read
//
// brief : adc to lux
//
// param : adc ->  adc value
//
// return : lux
extern uint16_t LOS_EvbLux_Convert(uint16_t adc)
{
  float Iss = (1.0 - adc/4095.0)*330.0;    //uA
  uint16_t lux =  (uint16_t)((double)Iss * 1.11);
  return lux;
}
