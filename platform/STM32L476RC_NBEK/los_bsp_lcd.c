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
#include "los_bsp_lcd.h"

#ifdef LOS_STM32L476xx

#include "stm32l4xx_hal.h"
#include "stm32l4xx_nbek.h"
#include "spi.h"
#include "tft128.h"
#endif

#ifdef LOS_STM32L476xx

/* sclk pin config*/
const PIN_HWAttrs  lcdSpiSck = 
{
  .pin = LCD_SPIx_SCK_PIN,
  .Port = LCD_SPIx_SCK_GPIO_PORT,
  .mode = GPIO_MODE_AF_PP,
  .pull = GPIO_NOPULL,
  .alternate = GPIO_AF5_SPI1,
  .state = GPIO_PIN_RESET
};
/* miso pin config*/
const PIN_HWAttrs  lcdSpiMiso = 
{
  .pin = LCD_SPIx_MISO_PIN,
  .Port = LCD_SPIx_MISO_MOSI_GPIO_PORT,
  .mode = GPIO_MODE_AF_PP,
  .pull = GPIO_NOPULL,
  .alternate = GPIO_AF5_SPI1,
  .state = GPIO_PIN_RESET
};

/* spi config*/
const SPI_HWAttrs   lcdSpiHWAttrs = 
{
  .sckPin = &lcdSpiSck,
  .misoPin = &lcdSpiMiso,
  .mosiPin = NULL,
  .csPin   = NULL
};

SPI_HandleTypeDef lcdSpiObj;

const SPI_Config  lcdSpiCfg = 
{
  .spiHandle = &lcdSpiObj,
  .spiHWAtrrs = &lcdSpiHWAttrs
};

/* LCD object */
LCD_Object lcdObject;

/* LCD hardware attribute structure */
const LCD_HWAttrs lcdHWAttrs = {
  .LCD_initCmd = &LCD_initCmd,
  .lcdModePin  = {LCD_CD_PIN,LCD_CD_GPIO_PORT,GPIO_MODE_OUTPUT_PP,GPIO_PULLUP,0,GPIO_PIN_RESET},       /* LCD mode pin  */
  .lcdCsnPin   = {LCD_CS_PIN,LCD_CS_GPIO_PORT,GPIO_MODE_OUTPUT_PP,GPIO_PULLUP,0,GPIO_PIN_RESET},       /* LCD CSn pin   */
  .spiIndex    = (SPI_Config*)&lcdSpiCfg
};

/* LCD configuration structure */
const LCD_Config LCD_config = {
  .object  = &lcdObject,
  .hwAttrs = &lcdHWAttrs
};

#endif
//*********************************************************************************
// fn : LOS_EvbLcdInit
//
// brief : Init the lcd
//
// param : none
//
// return : none
void LOS_EvbLcdInit(void)
{
#ifdef LOS_STM32L476xx
  lcdSpiObj.Instance = SPI1;
  lcdSpiObj.Init.Mode = SPI_MODE_MASTER;
  lcdSpiObj.Init.Direction = SPI_DIRECTION_2LINES;
  lcdSpiObj.Init.DataSize = SPI_DATASIZE_8BIT;
  lcdSpiObj.Init.CLKPolarity = SPI_POLARITY_LOW;
  lcdSpiObj.Init.CLKPhase = SPI_PHASE_1EDGE;
  lcdSpiObj.Init.NSS = SPI_NSS_SOFT;
  lcdSpiObj.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;//SPI_BAUDRATEPRESCALER_2;
  lcdSpiObj.Init.FirstBit = SPI_FIRSTBIT_MSB;
  lcdSpiObj.Init.TIMode         = SPI_TIMODE_DISABLE;
  lcdSpiObj.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  lcdSpiObj.Init.CRCPolynomial  = 7;
  lcdSpiObj.Init.CRCLength      = SPI_CRC_LENGTH_DATASIZE;
  lcdSpiObj.Init.NSSPMode       = SPI_NSS_PULSE_ENABLE;
  
  Board_SPI_Init(&lcdSpiCfg);
#endif
}
