//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_bsp_uart.c
//  Date     : 2018-10-28 14:06
//  Version  : V0001
// 历史记录  : 第一次创建
//******************************************************************************
#include "los_bsp_uart.h"
#include <stdarg.h>

#ifdef LOS_STM32L476xx
#include "stm32l4xx_nbek.h"
#include "gpio.h"
#include "usart.h"

//************************************************************************************
//配置串口硬件相关信息

const PIN_HWAttrs  logTxPin = 
{
  LOGx_TX_PIN,
  LOGx_TX_GPIO_PORT,
  GPIO_MODE_AF_PP,
  GPIO_NOPULL,
  LOGx_AF
};
const PIN_HWAttrs  logRxPin = 
{
  LOGx_RX_PIN,
  LOGx_RX_GPIO_PORT,
  GPIO_MODE_AF_PP,
  GPIO_NOPULL,
  LOGx_AF
};

const UART_HWAttrs uartLogHWAttrs = {
  .txPin     = &logTxPin,
  .rxPin     = &logRxPin,
};

static UART_HandleTypeDef UartHandle;

const UART_Config logUartCfg = {
  .pUart = &UartHandle,
  .hwAttrs = &uartLogHWAttrs
};
#endif

//------------------------------------------------------------------------------------
//************************************************************************************
// fn : LOS_EvbUartInit
//
// brief : Init the log uart 
//
// param : none
//
// return : none
void LOS_EvbUartInit(void)
{
#ifdef LOS_STM32L476xx
    UartHandle.Instance                    = LOGx;
    UartHandle.Init.BaudRate               = 115200;
    UartHandle.Init.WordLength             = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits               = UART_STOPBITS_1;
    UartHandle.Init.Parity                 = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode                   = UART_MODE_TX_RX;
    UartHandle.Init.OverSampling           = UART_OVERSAMPLING_16;
    UartHandle.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    Board_UART_Init(&logUartCfg);
    
#endif
}

//***************************************************************************************
// fn : LOS_EvbUartWriteByte
//
// brief : send one byte with uart
//
// param : c -> output
//
// return : none
void LOS_EvbUartWriteByte(char c)
{
#ifdef LOS_STM32L476xx
    if(HAL_UART_Transmit(&UartHandle, (uint8_t*)&c, 1, 5000)!= HAL_OK)
    {
      //Error_Handler();
    }
#endif
    return;
}
//***************************************************************************************
// fn : LOS_EvbUartWriteStr
//
// brief : send strings with uart
//
// param : str -> the reference of the output 
//
// return : none
void LOS_EvbUartWriteStr(const char* str)
{
#ifdef LOS_STM32L476xx
    while (*str)
    {
        if(HAL_UART_Transmit(&UartHandle, (uint8_t*)str, 1, 5000)!= HAL_OK)
        {
            //Error_Handler();
        }
        str++;
    }
#endif
    return;
}

#ifdef LOS_STM32L476xx
static char _buffer[128];
#endif
void LOS_EvbUartPrintf(char* fmt, ...)
{
#ifdef LOS_STM32L476xx
    int i;
    va_list ap;
    va_start(ap, fmt);
    vsprintf(_buffer, fmt, ap);
    va_end(ap);

    for (i = 0; _buffer[i] != '\0'; i++)
    {
        LOS_EvbUartWriteByte(_buffer[i]);
    }
#endif
    return;
}

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);
 
  return ch;
}
