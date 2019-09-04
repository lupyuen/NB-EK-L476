//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_bsp_sht2x.c
//  Date     : 2018-11-14 11:47
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#include "los_bsp_sht2x.h"
#include "los_sem.h"

#ifdef LOS_STM32L476xx

#include "stm32l4xx_nbek.h"

#endif

//******************************************************************************
// 常量定义区
//******************************************************************************
#define SHT20_IIC_ADDR              0x80

//
#define SHT20_READ_ADDR             0X81
#define SHT20_WRITE_ADDR            0x80

// CMD
#define SHT20_MEASURE_TEMP_CMD      0xE3
#define SHT20_MEASURE_RH_CMD        0xE5

#define SHT20_SOFT_RESET_CMD        0xFE


const I2C_HWAttrs  SHT2xHWAttrs =
{
  {
    SHT2x_I2C_SCL_PIN,
    SHT2x_I2C_SCL_PORT,
    SHT2x_I2C_GPIO_MODE,
    GPIO_PULLUP,
    SHT2x_I2C_AF,
    GPIO_PIN_RESET
  },
  {
    SHT2x_I2C_SDA_PIN,
    SHT2x_I2C_SDA_PORT,
    SHT2x_I2C_GPIO_MODE,
    GPIO_PULLUP,
    SHT2x_I2C_AF,
    GPIO_PIN_RESET
  }
};

I2C_HandleTypeDef hSht2xI2c;

const I2C_Config SHT2xCfg = 
{
  .pI2c = &hSht2xI2c,
  .hwAttrs = &SHT2xHWAttrs
};

SHT2x_Handle  sht2Handle = 
{
	.pI2cCfg = &SHT2xCfg,
	.semId = 0
};

//*********************************************************************************
// fn : SHT2x_Send
//
// brief : Send data
static void SHT2x_Send(uint8_t addr , uint8_t *pData, uint16_t len);

//*********************************************************************************
// fn : SHT2x_Rece
//
// brief : Rece data
static void SHT2x_Rece(uint8_t addr , uint8_t *pData, uint16_t len);

//*********************************************************************************
// fn : LOS_EvbSHT2x_Init
//
// brief : Init the sht2x
//
// param : none
//
// return : none
extern void LOS_EvbSHT2x_Init(void)
{
#ifdef LOS_STM32L476xx
  hSht2xI2c.Instance              = SHT2x_I2C;              //I2C2
  hSht2xI2c.Init.Timing           = 0x10909CEC;
  hSht2xI2c.Init.OwnAddress1      = 0;
  hSht2xI2c.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
  hSht2xI2c.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
  hSht2xI2c.Init.OwnAddress2      = 0;
  hSht2xI2c.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hSht2xI2c.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
  hSht2xI2c.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;
  
  LOS_SemCreate(1,&sht2Handle.semId);
  
  Board_I2C_Init(&SHT2xCfg);
#endif
}

//******************************************************************************
// fn : SHT20_ReadTemp
//
// brief : read tempture from sht20 with iic
//
// param : none
//
// return : 温度原始数据
uint16_t SHT20_ReadTemp(void)
{
  uint16_t temp = 0;
  uint8_t  cmd = SHT20_MEASURE_TEMP_CMD;
	uint8_t pDATA[3] = {0,0,0};
  //
	LOS_SemPend(sht2Handle.semId,LOS_WAIT_FOREVER);

  SHT2x_Send(SHT20_WRITE_ADDR,&cmd,1);
  

  SHT2x_Rece(SHT20_READ_ADDR,pDATA,3);
  
  temp = pDATA[0];
  temp <<= 8;
  temp += (pDATA[1] & 0xfc);
	
	LOS_SemPost(sht2Handle.semId);  
  //printf("temp=%04X\r\n",temp);
  return temp;
}

//******************************************************************************
// fn : SHT20_ReadRH
//
// brief : read RH from sht20 with iic
//
// param : none
//
// return : none
uint16_t SHT20_ReadRH(void)
{
  uint16_t rh = 0;
  uint8_t  cmd = SHT20_MEASURE_RH_CMD;
  uint8_t pDATA[3] = {0,0,0};
	
	LOS_SemPend(sht2Handle.semId,LOS_WAIT_FOREVER);
	
  SHT2x_Send(SHT20_WRITE_ADDR,&cmd,1);
  

  SHT2x_Rece(SHT20_READ_ADDR,pDATA,3);
  
  rh = pDATA[0];
  rh <<= 8;
  rh += (pDATA[1] & 0xf0);
	
 	LOS_SemPost(sht2Handle.semId);
	
  return rh;
}

//******************************************************************************
// fn : SHT20_SoftReset
//
// brief : softreset sht20
//
// param : none
//
// return : none
void SHT20_SoftReset(void)
{
  uint8_t  cmd = SHT20_MEASURE_RH_CMD;
  SHT2x_Send(SHT20_WRITE_ADDR,&cmd,1);
  
}
//******************************************************************************
// fn : SHT20_Convert
//
// brief : raw data to actual temperture or humiduty
//
// param : value-> raw data
//         isTemp-> >0 temperture : 0 humiduty
//
// return : temperture or humiduty
extern float SHT20_Convert(uint16_t value,uint8_t isTemp)
{
  float tmp = 0.0;
  if(isTemp)
  {
    tmp = -46.85 + (175.72* value)/(1 << 16);
  }
  else
  {
    tmp = -6 + (125.0 *value)/(1<<16);
  }
  return tmp;
}

//*********************************************************************************
// fn : SHT2x_Send
//
// brief : Send data
static void SHT2x_Send(uint8_t addr , uint8_t *pData, uint16_t len)
{
	Board_I2C_Send(&SHT2xCfg,addr,pData,len);
}

//*********************************************************************************
// fn : SHT2x_Rece
//
// brief : Rece data
static void SHT2x_Rece(uint8_t addr , uint8_t *pData, uint16_t len)
{
	Board_I2C_Rece(&SHT2xCfg,addr,pData,len);
}
