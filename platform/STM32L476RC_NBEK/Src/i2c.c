//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : i2c.c
//  Date     : 2018-11-14 11:10
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#include "i2c.h"
#include "los_hwi.h"
#include "los_event.h"
#ifdef LOS_STM32L476xx

#include "stm32l4xx_hal.h"
#include "stm32l4xx_nbek.h"
#include "stm32l4xx_it.h"
#include "gpio.h"
#endif


#define I2C2_SEND_END_EVT   0x0001
#define I2C2_RECE_END_EVT   0x0002

extern const I2C_Config SHT2xCfg;

/*事件控制结构体*/
//static EVENT_CB_S  i2c_event;
//-----------------------------------------------------------------------------
// Functions prototypes

/* trun on/off the i2c clock*/
static uint32_t I2C_Clk_OnOff(I2C_Config* i2cCfg,uint8_t opt);

//****************************************************************************
// fn : Board_I2C_Init
//
// brief : init a i2c periphral
//
// param : i2cCfg -> the reference of i2c_config
//
// return : HAL_OK or HAL_ERROR
extern uint32_t Board_I2C_Init(const I2C_Config* i2cCfg)
{
	UINT32 uwRet;
  if(i2cCfg == NULL)
  {
    return HAL_ERROR;
  }
  if (HAL_I2C_Init(i2cCfg->pI2c) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  /*
   * Configure Analogue filter 
   */
  if (HAL_I2CEx_ConfigAnalogFilter(i2cCfg->pI2c, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  /**Configure Digital filter 
   */
  if (HAL_I2CEx_ConfigDigitalFilter(i2cCfg->pI2c, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  return HAL_OK;
}
//****************************************************************************
// fn : Board_I2C_Send
//
// brief : Send the data
//
// param : i2cCfg -> the reference of i2c_config
//         addr ->  the slave addr
//         pdata -> the pointer of data
//         len -> the length
//
// return : HAL_OK or HAL_ERROR
extern uint32_t Board_I2C_Send(const I2C_Config* i2cCfg,uint8_t addr , uint8_t *pData, uint16_t len)
{
  HAL_StatusTypeDef state = HAL_ERROR;
  if(len == 0 || pData == 0)
  {
    return HAL_ERROR;
  }
	
  state =  HAL_I2C_Master_Transmit(i2cCfg->pI2c,addr,pData,len,100);
  
  return state;
}

//****************************************************************************
// fn : Board_I2C_Read
//
// brief : Rend the data
//
// param : i2cCfg -> the reference of i2c_config
//         addr ->  the slave addr
//         pdata -> the pointer of data
//         len -> the length
//
// return : HAL_OK or HAL_ERROR
extern uint32_t Board_I2C_Rece(const I2C_Config* i2cCfg,uint8_t addr , uint8_t *pData, uint16_t len)
{
  if(len == 0 || pData == 0)
  {
    return HAL_ERROR;
  }
  return HAL_I2C_Master_Receive(i2cCfg->pI2c,addr,pData,len,100);
}
//****************************************************************************************
// fn : HAL_I2C_MspInit
//
// brief : 由于系统自行调用。开发者不可调用
//
// param : i2cHandle -> the handle of i2c periphral
//
// return : none
void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(i2cHandle->Instance == I2C2)
  {
  
    /**I2C2 GPIO Configuration    
    PB13     ------> I2C2_SCL
    PB14     ------> I2C2_SDA 
    */
//    GPIO_InitStruct.Pin = SHT2x_I2C_SCL_PIN|SHT2x_I2C_SDA_PIN;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = SHT2x_I2C_AF;
//    HAL_GPIO_Init(SHT2x_I2C_PORT, &GPIO_InitStruct);
    
		Board_GPIO_OneInit((const PIN_HWAttrs*)&SHT2xCfg.hwAttrs->sclPin);
		Board_GPIO_OneInit((const PIN_HWAttrs*)&SHT2xCfg.hwAttrs->sdaPin);
    /* I2C2 clock enable */
    __HAL_RCC_I2C2_CLK_ENABLE();
    
    //LOS_HwiCreate(I2C2_EV_IRQn, 7,0,I2C2_EV_IRQHandler,0);
  }
}
//****************************************************************************************
// fn : HAL_I2C_MspDeInit
//
// brief : 由于系统自行调用。开发者不可调用
//
// param : i2cHandle -> the handle of i2c periphral
//
// return : none
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance== I2C2)
  {
    /* Peripheral clock disable */
    __HAL_RCC_I2C2_CLK_DISABLE();

    /**I2C2 GPIO Configuration    
    PB13     ------> I2C2_SCL
    PB14     ------> I2C2_SDA 
    */
    Board_GPIO_DeInit(SHT2xCfg.hwAttrs->sclPin.Port,SHT2xCfg.hwAttrs->sclPin.pin);
    Board_GPIO_DeInit(SHT2xCfg.hwAttrs->sdaPin.Port,SHT2xCfg.hwAttrs->sdaPin.pin);
//    HAL_GPIO_DeInit(SHT2x_I2C_PORT, SHT2x_I2C_SCL_PIN|SHT2x_I2C_SDA_PIN);
  
    /* I2C2 interrupt Deinit */  
    //LOS_HwiDelete(I2C2_EV_IRQn);
  }
}
//*****************************************************************************************
// fn : I2C_Clk_OnOff
//
// brief : trun on/off the i2c clock
//
// param : i2cCfg -> the reference of i2c_config
//         opt -> 0: disable; !0 : enable
//
// return : none
uint32_t I2C_Clk_OnOff(I2C_Config* i2cCfg,uint8_t opt)
{
  if(i2cCfg == NULL)
  {
    return HAL_ERROR;
  }
  if(i2cCfg->pI2c->Instance == I2C1)
  {
    if(opt)
    {
      __HAL_RCC_I2C1_CLK_ENABLE();
    }
    else
    {
      __HAL_RCC_I2C1_CLK_DISABLE();
    }
  }
  else if(i2cCfg->pI2c->Instance == I2C2)
  {
    if(opt)
    {
      __HAL_RCC_I2C2_CLK_ENABLE();
    }
    else
    {
      __HAL_RCC_I2C2_CLK_DISABLE();
    }
  }
  else if(i2cCfg->pI2c->Instance == I2C3)
  {
    if(opt)
    {
      __HAL_RCC_I2C3_CLK_ENABLE();
    }
    else
    {
      __HAL_RCC_I2C3_CLK_DISABLE();
    }
  }
  else
  {
    return HAL_ERROR;
  }
  return HAL_OK;
}
//**************************************************************************************
// fn : HAL_I2C_MasterTxCpltCallback
//
// brief : 中断回调函数
// 
// param : hi2c -> the reference of i2c
//
// return : none
//void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
//{
//  if(hi2c->Instance == SHT2x_I2C)
//  {
//    
//  }
//}

//**************************************************************************************
// fn : HAL_I2C_MasterRxCpltCallback
//
// brief : 中断回调函数
// 
// param : hi2c -> the reference of i2c
//
// return : none
//void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
//{
//  if(hi2c->Instance == SHT2x_I2C)
//  {
//    
//  }
//}
