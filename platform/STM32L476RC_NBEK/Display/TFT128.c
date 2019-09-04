/*
 * Copyright (c) 2015, Ghostyu Co. Ltd.,
 *          All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <string.h>
#include "los_typedef.h"
#include "los_hwi.h"
#include "los_mux.h"


//#include "lcd_base.h"
#include "spi.h"
#include "gpio.h"
#include "stm32l4xx_hal_gpio.h"

#include "TFT128.h"
#include "LCDDogm1286_util.h"

#include "start_logo.c"
/* macro to calculate minimum value */
#define MIN(a,b) (((a)<(b))?(a):(b))

/*convert the lcd buffer to ascii sizes*/
typedef short (*buf_asc)[32] ;

/* Externs */
extern const LCD_Config LCD_config;

/* Used to check status and initialization */
static int LCD_count = -1;

/* Static LCD functions */
static bool LCD_initHw(LCD_Handle handle);

static void LCD_initSet(LCD_Handle handle, const LCD_Command *pcCmd);

static void LCD_sendCommand(LCD_Handle handle, const char *pcCmd,
    unsigned char ucLen);

static void LCD_sendData(LCD_Handle handle, const char *pcData,
    unsigned short usLen);

//static void LCD_gotoXY(LCD_Handle handle, unsigned char ucX, unsigned char ucY);

static void LCD_setRegion(LCD_Handle handle,unsigned char ucX,unsigned char ucY,
    unsigned char ucXend,unsigned char ucYend);

static bool LCD_sendArray(LCD_Handle handle, const char *pcData,
    unsigned short usLen);

static void LCD_bufferLine(LCD_Handle handle,
    unsigned char ucXFrom, unsigned char ucYFrom, unsigned char ucXTo,
    unsigned char ucYTo, unsigned char ucDraw);

static void LCD_doBufferClearPage(LCD_Handle handle,LCD_Page iPage, bool blocking);

static void LCD_doBufferPrintString(LCD_Handle handle,
    const char *pcStr, unsigned char ucX, unsigned char ucY, bool blocking);

static void LCD_doBufferSetHLine(LCD_Handle handle, 
    unsigned char ucXFrom, unsigned char ucXTo, unsigned char ucY,
    bool blocking);

static void LCD_doBufferClearHLine(LCD_Handle handle,
    unsigned char ucXFrom, unsigned char ucXTo, unsigned char ucY,
    bool blocking);

static void LCD_doBufferSetVLine(LCD_Handle handle, 
    unsigned char ucX, unsigned char ucYFrom, unsigned char ucYTo,
    bool blocking);

static void LCD_doBufferClearVLine(LCD_Handle handle,
    unsigned char ucX, unsigned char ucYFrom, unsigned char ucYTo,
    bool blocking);

static void LCD_doBufferSetPx(LCD_Handle handle, unsigned char color,unsigned char ucX, unsigned char ucY);

static void LCD_fullRegion(LCD_Handle handle, Color color, unsigned char ucXFrom,
                           unsigned char ucYFrom, unsigned char ucXTo, unsigned char ucYTo);

static void LCD_Update(LCD_Handle handle,unsigned char ucXFrom,
                           unsigned char ucYFrom, unsigned char ucXTo, unsigned char ucYTo);


void LCD_delayMs(int ms)
{
  LOS_TaskDelay(LOS_MS2Tick(ms));
}
//******************************************************************************
// fn : LCD_close
//
// brief : Function assumes that the handle is not NULL
//
// param : none
//
// return : none
void LCDTFT_close(LCD_Handle handle)
{
    /* Get the pointers to the LCD object and buffer */
    LCD_Object *object = handle->object;
  
    LOS_MuxDelete(object->lcdMutex);
    /* Mark object as closed*/
    object->isOpen = false;
}

/*
 *  ======== LCD_init ========
 */
void LCD_init()
{
  /* Allow multiple calls for LCD_init */
  if (LCD_count >= 0)
  {
      return;
  }
  LCD_count++;

  LCD_Handle handle = (LCD_Handle)&(LCD_config);
  LCD_Object *object;

  /* Get the pointer to the object */
  object = handle->object;

  /* Mark the object as available */
  object->isOpen = 0;
}

/*
 *  ======== LCD_Params_init ========
 */
void LCDTFT_Params_init(LCD_Params *params)
{
    params->lcdWriteTimeout =  LOS_WAIT_FOREVER;
    params->spiBitRate = 10000000;               //10M bits/S
    params->CPOL = SPI_POLARITY_LOW;
    params->CPOL = SPI_PHASE_1EDGE;
}

/*
 *  ======== LCD_open ========
 */
LCD_Handle LCDTFT_open(LCD_Buffer *buffers, uint8_t nBuffers,
    LCD_Params *lcdParams)
{
    UINTPTR uvIntSave;
    LCD_Object *lcdObject;
    LCD_HWAttrs const *lcdHwAttrs;
    
    LCD_Handle handle = (LCD_Handle) &(LCD_config);

    /* Get the pointer to the object and hwAttrs. */
    lcdObject = handle->object;
    lcdHwAttrs = handle->hwAttrs;
  
    /* Disable preemption while checking if the LCD is open. */ 
    uvIntSave = LOS_IntLock();
    /* Determine if the device index was already opened. */
    if (lcdObject->isOpen == TRUE)
    {
      LOS_IntRestore(uvIntSave);
      return (NULL);
    }

    /* Mark the handle as being used. */
    lcdObject->isOpen = TRUE;
    LOS_IntRestore(uvIntSave);
    
    /* If buffers are NULL, or nBuffers <= 0, return */
    if((buffers == NULL) ||(nBuffers <= 0))
    {
        return (NULL);
    }
    
    // Initialize the LCD object
    lcdObject->spiHandle = lcdHwAttrs->spiIndex->spiHandle;
    lcdObject->lcdWriteTimeout = lcdParams->lcdWriteTimeout;
    lcdObject->lcdBuffers = buffers;
    lcdObject->nBuffers = nBuffers;
    
    LOS_MuxCreate(&lcdObject->lcdMutex);
    /* Create a counting semaphore for each buffer */

    /* Configure the hardware module */
    if(!LCD_initHw(handle))
    {
       //Hw initialization failed
       return (NULL);
    }

    /* Send LCD init commands */
    LCD_initSet(handle, (const LCD_Command *) (lcdHwAttrs->LCD_initCmd));

    return (handle);
}

static void LCD_initSet(LCD_Handle handle,const LCD_Command* pcCmd)
{
  unsigned int cmdLen = 0;
  
  LCD_sendCommand(handle,&pcCmd->sleepExit.cmdId,1);
  
  /* Delay ~120 ms for LCD to be wake up. */
  LCD_delayMs(pcCmd->sleepExit.delayMs);

  cmdLen += sizeof(LCD_Sleep_CMD);
  
  LCD_Sleep_CMD *pSleepCmd = (LCD_Sleep_CMD *)pcCmd;
  LCD_Param_CMD *pParamCmd = (LCD_Param_CMD *)(pSleepCmd + 1);
  while(cmdLen < sizeof(LCD_Command))
  {
    LCD_sendCommand(handle,&pParamCmd->cmdId,1);
    if(pParamCmd->count > 0)
    {
      LCD_sendData(handle,pParamCmd->param,pParamCmd->count);
    }
    pParamCmd++;
    cmdLen += sizeof(LCD_Param_CMD);
  }
}
/*
 *  ======== LCD_writeLine ========
 *  This function writes one line of the specified buffer
 *  and sends it to the display.
 */
void LCDTFT_writeLine(LCD_Handle handle, char *str,
    unsigned int uiValue, unsigned char ucFormat, unsigned char ucLine)
{
  LCD_Page pageNo = (LCD_Page) (ucLine % LCD_PAGE_COUNT);
   UINT32  uwRet;
  LCD_Object *object = handle->object;
  /* ÉêÇë»¥³âËø*/
  uwRet=LOS_MuxPend(object->lcdMutex, object->lcdWriteTimeout);
  
  if(uwRet == LOS_ERRNO_MUX_TIMEOUT)
  {
    return;
  }
    if (ucFormat)
    {
        unsigned char maxLen = 50; // max string length
        unsigned char buf[50];
        unsigned char strLength;

        /* Check that there is a null termination in the string */
        const char *end = (const char *) memchr(str, '\0', maxLen);
        if (end == NULL)
            strLength = maxLen;
        else
            strLength = end - str;

        memset(buf, ' ', maxLen);
        memcpy(buf, str, strLength);

        /* Get number of characters in string */
        unsigned char ucNumOfDigits = LCD_getIntLength(uiValue, ucFormat);

        /* String length + 2 spaces + number of digits cannot exceed one line */
        if ((strLength + 2 + ucNumOfDigits) > (LCD_COLS / LCD_CHAR_WIDTH))
        {
            return;
        }

        /* Copy to local buffer and send */
        _itoa(uiValue, &buf[strLength + 2], ucFormat);

        /* Pend on buffer semaphore */


        /* Clear the page */
        LCD_doBufferClearPage(handle,  pageNo, false);

        /* write buffer*/
        LCD_doBufferPrintString(handle, (char*) buf, 0, pageNo,
                false);

    }
    else
    {
        /* Clear the page */
        LCD_doBufferClearPage(handle, pageNo, false);
        /* write buffer*/
        LCD_doBufferPrintString(handle, str, 0, pageNo, false);

    }
  LOS_MuxPost(object->lcdMutex);
}
//******************************************************************************
// fn : LCD_bufferClear
//
// brief : This function empties the specified LCD buffer by filling it with zeros.
//
// params : handle -> the lcd handle
//
// return : none
void LCDTFT_bufferClear(LCD_Handle handle)
{
   UINT32  uwRet;
  LCD_Object *object = handle->object;
  /* ÉêÇë»¥³âËø*/
  uwRet=LOS_MuxPend(object->lcdMutex, object->lcdWriteTimeout);
  
  if(uwRet == LOS_ERRNO_MUX_TIMEOUT)
  {
    return;
  }
  LCD_fullRegion(handle,handle->object->pColorInfo->blackColor,0,0,LCD_COLS - 1,LCD_ROWS - 1);
  LOS_MuxPost(object->lcdMutex);
}
//******************************************************************************
// fn : LCD_bufferStartCover
//
// brief : This function display a logo
//
// params : handle -> the lcd handle
//
// return : none

void LCDTFT_bufferStartCover(LCD_Handle handle)
{
   UINT32  uwRet;
   /* Get the pointer to the LCD object*/
  LCD_HWAttrs const *hwAttrs = handle->hwAttrs;
  LCD_Object *object = handle->object;
  /* ÉêÇë»¥³âËø*/
  uwRet=LOS_MuxPend(object->lcdMutex, object->lcdWriteTimeout);
  
  if(uwRet == LOS_ERRNO_MUX_TIMEOUT)
  {
    return;
  }
  //ÉèÖÃÏÔÊ¾ÇøÓò 
  LCD_setRegion(handle,0,0,127,127);
  /* Set LCD mode signal (data) */
  HAL_GPIO_WritePin(hwAttrs->lcdModePin.Port, hwAttrs->lcdModePin.pin, GPIO_PIN_SET);
  /* Set LCD CSn low (spi active) */
  HAL_GPIO_WritePin(hwAttrs->lcdCsnPin.Port, hwAttrs->lcdCsnPin.pin, GPIO_PIN_RESET);

  LCD_sendArray(handle, (char const*)gImage_start_logo, 32768);
  HAL_GPIO_WritePin(hwAttrs->lcdCsnPin.Port, hwAttrs->lcdCsnPin.pin, GPIO_PIN_SET);
  
  LOS_MuxPost(object->lcdMutex);
  //delay 1s
  LOS_TaskDelay(LOS_MS2Tick(1000));

}

/*
 * ======== LCD_bufferClearPage ========
 * This function clears the specified page of a buffer
 */
void LCDTFT_bufferClearPage(LCD_Handle handle, LCD_Page iPage)
{
    UINT32 uwRet;
    LCD_Object *object = handle->object;
    /* ÉêÇë»¥³âËø*/
    uwRet=LOS_MuxPend(object->lcdMutex, object->lcdWriteTimeout);
    
    if(uwRet == LOS_ERRNO_MUX_TIMEOUT)
    {
      return;
    }   
    /* Call clear page function with use of semaphore */
    LCD_doBufferClearPage(handle, iPage, true);
    LOS_MuxPost(object->lcdMutex);
}

/*
 * ======== LCD_bufferClearPart ========
 * This function clears the pixels in a given piece of a page.
 */
void LCDTFT_bufferClearPart(LCD_Handle handle,
    unsigned char ucXFrom, unsigned char ucXTo, LCD_Page iPageFrom,
    LCD_Page iPageTo)
{
  unsigned char ucYFrom,ucYTo;
  UINT32 uwRet;
  LCD_Object *object = handle->object;
  /* ÉêÇë»¥³âËø*/
  uwRet=LOS_MuxPend(object->lcdMutex, object->lcdWriteTimeout);
  
  if(uwRet == LOS_ERRNO_MUX_TIMEOUT)
  {
    return;
  }
  ucYFrom = iPageFrom * LCD_PAGE_ROWS;
  
  if(iPageTo  >=  LCD_PAGE_COUNT)
  {
    iPageTo = LCD_PAGE7; 
  }
  ucYTo = (iPageTo + 1) * LCD_PAGE_ROWS - 1;
  /* Clear buffer part */   
  LCD_fullRegion(handle,handle->object->pColorInfo->blackColor,ucXFrom,ucYFrom,ucXTo,ucYTo);
  
  LOS_MuxPost(object->lcdMutex);
}
/*
 *  ======== LCD_bufferPrintString ========
 *  This function writes a string to the specified buffer
 */
void LCDTFT_bufferPrintString(LCD_Handle handle, const char *pcStr,
                              unsigned char ucX, LCD_Page iPage)
{
  unsigned char ucY = 0;
  UINT32 uwRet;
  LCD_Object *object = handle->object;
  /* ÉêÇë»¥³âËø*/
  uwRet=LOS_MuxPend(object->lcdMutex, object->lcdWriteTimeout);
  
  if(uwRet == LOS_ERRNO_MUX_TIMEOUT)
  {
    return;
  }
  ucY = iPage * LCD_PAGE_ROWS;
  
  /* Pend on the semaphore if blocking option is set*/

  
  /* Call print string function with use of semaphore */
  LCD_doBufferPrintString(handle, pcStr, ucX, ucY, true);
  LOS_MuxPost(object->lcdMutex);
}

/*
 *  ======== LCD_bufferSetLine ========
 *  This function draws a line into the specified buffer.
 */
void LCDTFT_bufferSetLine(LCD_Handle handle, unsigned char ucXFrom,
                       unsigned char ucYFrom, unsigned char ucXTo, unsigned char ucYTo)
{
  UINT32 uwRet;
  LCD_Object *object = handle->object;
  /* ÉêÇë»¥³âËø*/
  uwRet = LOS_MuxPend(object->lcdMutex, object->lcdWriteTimeout);
  
  if(uwRet == LOS_ERRNO_MUX_TIMEOUT)
  {
    return;
  }
  /* Draw line */
  LCD_bufferLine(handle, ucXFrom, ucYFrom, ucXTo, ucYTo, 1);
  LOS_MuxPost(object->lcdMutex);  
}

/*
 *  ======== LCD_bufferClearLine ========
 *  This function clears a line intoo the specified buffer.
 */
void LCDTFT_bufferClearLine(LCD_Handle handle, unsigned char ucXFrom,
                          unsigned char ucYFrom, unsigned char ucXTo, unsigned char ucYTo)
{
  UINT32 uwRet;
  LCD_Object *object = handle->object;
  /* ÉêÇë»¥³âËø*/
  uwRet=LOS_MuxPend(object->lcdMutex, object->lcdWriteTimeout);
  
  if(uwRet == LOS_ERRNO_MUX_TIMEOUT)
  {
    return;
  }
  /* Clear line */
  LCD_bufferLine(handle, ucXFrom, ucYFrom, ucXTo, ucYTo, 0);
  LOS_MuxPost(object->lcdMutex); 
}

/*
 *  ======== LCD_bufferSetHLine ========
 *  This function draws a horizontal line into the specified buffer.
 */
void LCDTFT_bufferSetHLine(LCD_Handle handle, unsigned char ucXFrom,
                        unsigned char ucXTo, unsigned char ucY)
{
  UINT32 uwRet;
  LCD_Object *object = handle->object;
  /* ÉêÇë»¥³âËø*/
  uwRet=LOS_MuxPend(object->lcdMutex, object->lcdWriteTimeout);
  
  if(uwRet == LOS_ERRNO_MUX_TIMEOUT)
  {
    return;
  }
    /* Call LCD_doBufferSetHLine with use of semaphore */
  LCD_doBufferSetHLine(handle, ucXFrom, ucXTo, ucY, true);
  LOS_MuxPost(object->lcdMutex); 
}

/*
 *  ======== LCD_bufferClearHLine ========
 *  This function clears a horizontal line from the specified buffer.
 */
void LCDTFT_bufferClearHLine(LCD_Handle handle, unsigned char ucXFrom,
                          unsigned char ucXTo, unsigned char ucY)
{
  UINT32 uwRet;
  LCD_Object *object = handle->object;
  /* ÉêÇë»¥³âËø*/
  uwRet=LOS_MuxPend(object->lcdMutex, object->lcdWriteTimeout);
  
  if(uwRet == LOS_ERRNO_MUX_TIMEOUT)
  {
    return;
  }
  /* Call LCD_doBufferClearHLine with use of semaphore */
  LCD_doBufferClearHLine(handle, ucXFrom, ucXTo, ucY, true);
  LOS_MuxPost(object->lcdMutex); 
}

/*
 *  ======== LCD_bufferSetVLine ========
 *  This function draws a vertical line into the specified buffer.
 */
void LCDTFT_bufferSetVLine(LCD_Handle handle, unsigned char ucX,
                        unsigned char ucYFrom, unsigned char ucYTo)
{
  UINT32 uwRet;
  LCD_Object *object = handle->object;
  /* ÉêÇë»¥³âËø*/
  uwRet=LOS_MuxPend(object->lcdMutex, object->lcdWriteTimeout);
  
  if(uwRet == LOS_ERRNO_MUX_TIMEOUT)
  {
    return;
  }
  /* Call LCD_doBufferSetVLine with use of semaphore */
  LCD_doBufferSetVLine(handle,ucX, ucYFrom, ucYTo, true);
  LOS_MuxPost(object->lcdMutex); 
}

/*
 *  ======== LCD_bufferClearVLine ========
 *  This function clears a vertical line from the specified buffer.
 */
void LCDTFT_bufferClearVLine(LCD_Handle handle,  unsigned char ucX,
                          unsigned char ucYFrom, unsigned char ucYTo)
{
  UINT32 uwRet;
  LCD_Object *object = handle->object;
  /* ÉêÇë»¥³âËø*/
  uwRet=LOS_MuxPend(object->lcdMutex, object->lcdWriteTimeout);
  
  if(uwRet == LOS_ERRNO_MUX_TIMEOUT)
  {
    return;
  }
  /* Call LCD_doBufferClearVLine with use of semaphore */
  LCD_doBufferClearVLine(handle,  ucX, ucYFrom, ucYTo, true);
  LOS_MuxPost(object->lcdMutex); 
}

/*
 *  ======== LCD_bufferHArrow ========
 *  This function draws a horizontal arrow to the specified buffer.
 */
void LCDTFT_bufferHArrow(LCD_Handle handle,  unsigned char ucXFrom,
                      unsigned char ucXTo, unsigned char ucY)
{
  UINT32 uwRet;
  LCD_Object *object = handle->object;
  /* ÉêÇë»¥³âËø*/
  uwRet=LOS_MuxPend(object->lcdMutex, object->lcdWriteTimeout);
  
  if(uwRet == LOS_ERRNO_MUX_TIMEOUT)
  {
    return;
  }

  if (ucXTo > ucXFrom)
  {
    /* Draw left-to-right arrow */
    LCD_doBufferSetHLine(handle,  ucXFrom, ucXTo, ucY, false);
    LCD_doBufferSetVLine(handle,  ucXTo - 1, ucY - 1, ucY + 1,
            false);
    LCD_doBufferSetVLine(handle,  ucXTo - 2, ucY - 2, ucY + 2,
            false);
  }
  else if (ucXTo < ucXFrom)
  {
    /* Draw right-to-left arrow */
    LCD_doBufferSetHLine(handle,  ucXTo, ucXFrom, ucY, false);
    LCD_doBufferSetVLine(handle,  ucXTo + 1, ucY - 1, ucY + 1,
            false);
    LCD_doBufferSetVLine(handle,  ucXTo + 2, ucY - 2, ucY + 2,
            false);
  }
  LOS_MuxPost(object->lcdMutex);
}

/*
 *  ======== LCD_bufferVArrow ========
 *  This function draws a vertical arrow to the specified buffer.
 */
void LCDTFT_bufferVArrow(LCD_Handle handle,  unsigned char ucX,
                      unsigned char ucYFrom, unsigned char ucYTo)
{
  UINT32 uwRet;
  LCD_Object *object = handle->object;
  /* ÉêÇë»¥³âËø*/
  uwRet=LOS_MuxPend(object->lcdMutex, object->lcdWriteTimeout);
  
  if(uwRet == LOS_ERRNO_MUX_TIMEOUT)
  {
    return;
  }

  /* Draw the line */
  LCD_doBufferSetVLine(handle, ucX, ucYFrom, ucYTo, false);

  /* Draw arrowhead */
  LCD_doBufferSetHLine(handle,  ucX - 1, ucX + 1, ucYTo - 1, false);
  LCD_doBufferSetHLine(handle,  ucX - 2, ucX + 2, ucYTo - 2, false);
  LOS_MuxPost(object->lcdMutex);
}

/*
 *  ======== LCD_bufferSetPx ========
 *  This function sets a pixel in the specified buffer.
 */
void LCDTFT_bufferSetPx(LCD_Handle handle, unsigned char ucX, unsigned char ucY)
{
  UINT32 uwRet;
  LCD_Object *object = handle->object;
  /* ÉêÇë»¥³âËø*/
  uwRet=LOS_MuxPend(object->lcdMutex, object->lcdWriteTimeout);
  
  if(uwRet == LOS_ERRNO_MUX_TIMEOUT)
  {
    return;
  }
  /* Call LCD_doBufferSetPx with use of semaphore */
  LCD_doBufferSetPx(handle,true, ucX, ucY);
  LOS_MuxPost(object->lcdMutex); 
}

//******************************************************************************
// fn : LCD_hwInit
//
// brief : This functions initializes the LCD hardware module.It returns true 
//         if initialization was successful, false otherwise
// 
// param : handle -> the lcd handle
//         pcData -> point to the address of data area
//         usLen  -> the data len
//
// return : none
static bool LCD_initHw(LCD_Handle handle)
{
    /* Locals */ 
    LCD_HWAttrs const *hwAttrs;

    /* get the pointer to the hwAttrs */
    hwAttrs = handle->hwAttrs;
    Board_GPIO_OneInit(&hwAttrs->lcdCsnPin);
    Board_GPIO_OneInit(&hwAttrs->lcdModePin);
  
    HAL_GPIO_WritePin(hwAttrs->lcdModePin.Port,hwAttrs->lcdModePin.pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(hwAttrs->lcdCsnPin.Port,hwAttrs->lcdCsnPin.pin,GPIO_PIN_SET);

    return (true);
}

//******************************************************************************
// fn : LCD_sendCommand
//
// brief : This function sends @e ucLen bytes of commands to the LCD controller.
// 
// param : handle -> the lcd handle
//         pcData -> point to the address of data area
//         usLen  -> the data len
//
// return : none
static void LCD_sendCommand(LCD_Handle handle, const char *pcCmd, unsigned char ucLen)
{
    /* Get the pointer to the LCD object*/
    LCD_HWAttrs const *hwAttrs = handle->hwAttrs;

    /* Set LCD mode signal low (command) */
    HAL_GPIO_WritePin(hwAttrs->lcdModePin.Port,hwAttrs->lcdModePin.pin,GPIO_PIN_RESET);
    
    /* Set LCD CSn low (spi active) */
    HAL_GPIO_WritePin(hwAttrs->lcdCsnPin.Port,hwAttrs->lcdCsnPin.pin,GPIO_PIN_RESET);
    /* Do SPI transfer */
    if (LCD_sendArray(handle, pcCmd, ucLen)) {
        /* Clear CSn */
        HAL_GPIO_WritePin(hwAttrs->lcdCsnPin.Port,hwAttrs->lcdCsnPin.pin,GPIO_PIN_SET);
    }

}
//******************************************************************************
// fn : LCD_sendData
//
// brief : the send display data to lcd
// 
// param : handle -> the lcd handle
//         pcData -> point to the address of data area
//         usLen  -> the data len
//
// return : none
static void LCD_sendData(LCD_Handle handle, const char *pcData, unsigned short usLen)
{
    /* Get the pointer to the LCD object*/
    LCD_HWAttrs const *hwAttrs = handle->hwAttrs;
    /* Set LCD mode signal (data) */
    //PIN_setOutputValue(hPin, hwAttrs->lcdModePin, 1);
    HAL_GPIO_WritePin(hwAttrs->lcdModePin.Port,hwAttrs->lcdModePin.pin,GPIO_PIN_SET);

    /* Set LCD CSn low (spi active) */
    //PIN_setOutputValue(hPin, hwAttrs->lcdCsnPin, 0);
    HAL_GPIO_WritePin(hwAttrs->lcdCsnPin.Port,hwAttrs->lcdCsnPin.pin,GPIO_PIN_RESET);
    /* Do SPI transfer */
    if (LCD_sendArray(handle, pcData, usLen)) {
        /* Clear CSn */
        //PIN_setOutputValue(hPin, hwAttrs->lcdCsnPin, 1);
        HAL_GPIO_WritePin(hwAttrs->lcdCsnPin.Port ,hwAttrs->lcdCsnPin.pin,GPIO_PIN_SET);
    }
}

//******************************************************************************
// fn : LCD_gotoXY
//
// brief : Function that sets the internal data cursor of the LCD to the
//         location specified by @e ucX and @e ucY. When data is sent to the 
//         display, data will start printing at internal cursor location.
//
// param : handle -> the lcd handle
//         ucX    -> x position
//         ucY    -> y postion
//
// return :none
//static void LCD_gotoXY(LCD_Handle handle, unsigned char ucX, unsigned char ucY)
//{
//  LCD_setRegion(handle,ucX,ucY,ucX,ucY);
//}
//******************************************************************************
// fn : LCD_setRegion
//
// brief : Function that sets the internal display area. 
//
// param : handle -> the lcd handle
//         ucX    -> x position
//         ucY    -> y postion
//         ucXend -> the end x position
//         ucYend -> the end y position
//
// return :none
static void LCD_setRegion(LCD_Handle handle,unsigned char ucX,unsigned char ucY,
    unsigned char ucXend,unsigned char ucYend)
{

  unsigned char tmp[] = { 0x00, 0x00, 0x00,0x00 }; 
  tmp[0] = 0x2a;
  LCD_sendCommand(handle, (char *) tmp, 1);    //row curor
  tmp[0] = 0X00;
  tmp[1] = ucX + 2;
  tmp[2] = 0x00;
  tmp[3] = ucXend + 2;
  LCD_sendData(handle,(char const*)tmp,4);
  
  tmp[0] = 0X2b;                              //column curor
  LCD_sendCommand(handle, (char *) tmp, 1); 
  tmp[0] = 0X00;
  tmp[1] = ucY + 3;
  tmp[2] = 0x00;
  tmp[3] = ucYend + 3;
	LCD_sendData(handle,(char const*)tmp,4);
  
  tmp[0] = 0x2c;                              
  LCD_sendCommand(handle, (char *) tmp, 1); 
  
}
//******************************************************************************
// fn : LCD_fullRegin
//
// brief : full the speified area with color
//
// param : handle ->  the lcd handle
//         color  ->  the color
//         ucXFrom ->  x start points
//         ucYFrom ->  y start points
//         ucXTo   ->  x end points
//         ucYTo   ->  y end points
//
// return 
static void LCD_fullRegion(LCD_Handle handle, Color color, unsigned char ucXFrom,
                           unsigned char ucYFrom, unsigned char ucXTo, unsigned char ucYTo)
{
  
   /* Get the pointer to the LCD object*/
  LCD_HWAttrs const *hwAttrs = handle->hwAttrs;
  LCD_Object *object = handle->object;
  
  unsigned short i;
  unsigned short numPoint = 0;
  unsigned short  times = 0;
  unsigned short  remainPoint = 0;
  
  numPoint = (ucXTo  - ucXFrom + 1) * (ucYTo - ucYFrom + 1);
  
  LCD_setRegion(handle,ucXFrom,ucYFrom,ucXTo,ucYTo);
  
  for(i = 0; i < 512; i++)
  {
    *(object->lcdBuffers[0].pcBuffer + i) = color;
  }
  
  /* Set LCD mode signal (data) */
  HAL_GPIO_WritePin(hwAttrs->lcdModePin.Port,hwAttrs->lcdModePin.pin,GPIO_PIN_SET);
 
  /* Set LCD CSn low (spi active) */
  HAL_GPIO_WritePin(hwAttrs->lcdCsnPin.Port,hwAttrs->lcdCsnPin.pin,GPIO_PIN_RESET);
  
  times = numPoint / 512;
  remainPoint = numPoint % 512;
    
  for( i = 0 ;i < times; i++)
  {
    LCD_sendArray(handle, (char const*)object->lcdBuffers[0].pcBuffer, 1024);
  }
  if(remainPoint > 0)
  {
    LCD_sendArray(handle, (char const*)object->lcdBuffers[0].pcBuffer, remainPoint * 2);
  }
  
  HAL_GPIO_WritePin(hwAttrs->lcdCsnPin.Port,hwAttrs->lcdCsnPin.pin,GPIO_PIN_SET); 
  
}
/*
 *  ======== LCD_sendArray ========
 *  This function sends @e usLen bytes from starting from address
 *  @e pcData over SPI to the LCD controller. This function only pushes
 *  data to the SPI module. It does not manipulate the LCD display's
 *  CSn signal, nor the LCD mode signal (A0).
 */
static bool LCD_sendArray(LCD_Handle handle, const char *pcData, unsigned short usLen)
{
  
  /* Get the pointer to the LCD object*/
  LCD_Object *object = handle->object;
  
  /* Do SPI transfer */
  HAL_StatusTypeDef ret = HAL_SPI_Transmit(object->spiHandle,(uint8_t*)pcData,usLen,HAL_MAX_DELAY);
  if(ret != HAL_OK)
  {
    return false;
  }
  /* return... */
  return true;
}

/*
 *  ======== LCD_bufferLine ========
 *  Local function. Draws or clears (based on @e ucDraw) a line from
 *  @e (ucXFrom,ucYFrom) to @e (ucXTo,ucYTo). Uses Bresenham's line algorithm.
 */
static void LCD_bufferLine(LCD_Handle handle, unsigned char ucXFrom,
                           unsigned char ucYFrom, unsigned char ucXTo, unsigned char ucYTo, unsigned char ucDraw)
{
  signed char cX, cY, cDeltaY, cDeltaX, cD;
  signed char cXDir, cYDir;
  
  if (ucXFrom == ucXTo)
  {
    /* Vertical line */
    if (ucDraw)
    {
      LCD_doBufferSetVLine(handle,  ucXFrom, ucYFrom, ucYTo,
                           false);
    }
    else
    {
      LCD_doBufferClearVLine(handle,  ucXFrom, ucYFrom, ucYTo,
                             false);
    }
  }
  else if (ucYFrom == ucYTo)
  {
    /* Horizontal line */
    if (ucDraw)
    {
      LCD_doBufferSetHLine(handle,  ucXFrom, ucXTo, ucYFrom,
                           false);
    }
    else
    {
      LCD_doBufferClearHLine(handle,  ucXFrom, ucXTo, ucYFrom,
                             false);
    }
  }
  else
  {
    
    /* Diagonal Line => Bresenham's algorithm
    * Determine X and Y direction
    */
    cXDir = (ucXFrom > ucXTo) ? -1 : 1;
    cYDir = (ucYFrom > ucYTo) ? -1 : 1;
    
    /* Set start position and calculate X and Y delta */
    cX = ucXFrom;
    cY = ucYFrom;
    cDeltaY = ucYTo - ucYFrom;
    cDeltaX = ucXTo - ucXFrom;
    
    /* Take absolute value of X and Y delta */
    if (cDeltaY < 0)
    {
      cDeltaY *= -1;
    }
    if (cDeltaX < 0)
    {
      cDeltaX *= -1;
    }
    
    /* Determine principal direction and draw line */
    if (cDeltaX >= cDeltaY)
    {
      cD = (cDeltaY << 1) - cDeltaX;
      while (cX != ucXTo)
      {
        LCD_doBufferSetPx(handle,ucDraw,  cX, cY );
        
        if (cD < 0)
        {
          cD += (cDeltaY << 1);
        }
        else
        {
          cD += ((cDeltaY - cDeltaX) << 1);
          cY += cYDir;
        }
        cX += cXDir;
      }
    }
    else
    {
      cD = (cDeltaX << 1) - cDeltaY;
      while (cY != ucYTo)
      {
        
        LCD_doBufferSetPx(handle, ucDraw, cX, cY);
        if (cD < 0)
        {
          cD += (cDeltaX << 1);
        }
        else
        {
          cD += ((cDeltaX - cDeltaY) << 1);
          cX += cXDir;
        }
        cY += cYDir;
      }
    }
  }
}
//******************************************************************************
// fn : LCD_doBufferClearPage
//
// brief : This function clears the page specified by @e iPage in the given buffer
//         If blocking is set to true, the task execution will be blocked until all
//         buffer modifications have finished.
//
// param : handle -> the lcd hanle
//         iPage -> the line
//         blocking -> 
//
// return : none
static void LCD_doBufferClearPage(LCD_Handle handle,
                                 LCD_Page iPage, bool blocking)
{
    unsigned char ucYFrom;

    /* Get pointer to the LCD object */
    LCD_Object *object = handle->object;
 

    /* Pend on the semaphore if blocking option is set*/

    /* Clear page in buffer */
    ucYFrom = iPage * LCD_COLS;
    
    LCD_fullRegion(handle,object->pColorInfo->blackColor,0,ucYFrom,127,ucYFrom + LCD_COLS);

}

//******************************************************************************
// fn : LCD_doBufferPrintString
//
// brief : This function writes a string to the specified buffer,If blocking is 
//         set to true, the task execution will be blocked until all buffer 
//         modifications have finished.
//
// param : handle -> the lcd hanle
//         pcStr -> the address of display string
//         ucX -> the x position
//         ucY -> the y position
//         blocking -> whether blocking or not 
//
// return : none
static void LCD_doBufferPrintString(LCD_Handle handle, const char *pcStr,
                                    unsigned char ucX, unsigned char ucY, bool blocking)
{
  
  unsigned char i,j,ucI;
  unsigned char k;
  unsigned char raw;
  unsigned char ucStrSize = LCD_getStringLength(pcStr);

  /* Get pointer to the LCD object */
  LCD_Object *object = handle->object;
  
  buf_asc asc = (buf_asc)(object->lcdBuffers[0].pcBuffer);
  
  
  raw = ucX;
  ucX >>= 3;
  /* Running through each letter in input string */
  for(ucI = 0; (ucI < ucStrSize) && (ucI + ucX) < MAX_CHAR_NUMM ; ucI++)
  {
    k = pcStr[ucI];

    if (k > 32)                                //¿ÉÏÔÊ¾×Ö·ûÊÇ´Óascii=32 ¿ªÊ¼
    {
      k -= 32; 
    }
    else 
    {
      k = 0;                                   //²»¿ÉÏÔÊ¾×Ö·û¶¼ÒÔ¿Õ¸ñÏÔÊ¾
    }
    
    for(i = 0; i < 16 ; i++)
    {
      for( j = 0 ; j < 8 ; j++) 
      {
        if(LCD_alphabet[k * 16 + i] & (0x80 >> j))	
        {
          asc[i][(ucI % 4)*8 +j] = object->pColorInfo->frontColor;
        }
        else 
        {
          asc[i][(ucI % 4)*8 +j] = object->pColorInfo->blackColor;
        }
      }
    }
    //512ÏÔ´æµã£¬Ö»ÄÜ´æ·Å4¸ö16*8³ß´ç´óÐ¡µÄ×Ö·û
    if((ucI % 4) == 3)
    {
      LCD_Update(handle,raw,ucY, raw + 31 ,ucY + LCD_PAGE_ROWS -1);
      raw += 32;
    }
  }
  
  k = ucI % 4;
  if(k)
  {
    //½«ÓàÏÂµÄÏÔ´æ¿Õ¼ä£¬È«²¿ÉèÖÃ³É±³¾°É« 
    for(char x = k  ; x < 4 ; x++)
    {
      for(i = 0; i < 16 ; i++)
      {
        for( j = 0 ; j < 8; j++) 
        {
          asc[i][x*8 +j] = object->pColorInfo->blackColor;
        }
      }
    }
    LCD_Update(handle,raw,ucY,raw + 31,ucY + LCD_PAGE_ROWS -1);
  }
  
}
//******************************************************************************
// fn : LCD_Update
// 
// brief : copy the memery to lcd static memery.the display memery size is 1024 
//          bytes. so,the region size must be 512 pixels
//
// params : handle    -> the lcd handle
//          ucXFrom   -> the start x point
//          ucYFrom   -> the start y point
//          ucXTo     -> the end x point
//          ucYTo     -> the end y point
// return : none
static void LCD_Update(LCD_Handle handle,unsigned char ucXFrom,
                           unsigned char ucYFrom, unsigned char ucXTo, unsigned char ucYTo)
{
  
   /* Get the pointer to the LCD object*/
  LCD_HWAttrs const *hwAttrs = handle->hwAttrs;
  LCD_Object *object = handle->object;
  
  unsigned short numPoint = 0;
  
  numPoint = (ucXTo  - ucXFrom + 1) * (ucYTo - ucYFrom + 1);
  
  LCD_setRegion(handle,ucXFrom,ucYFrom,ucXTo,ucYTo);
  
  /* Set LCD mode signal (data) */
  HAL_GPIO_WritePin(hwAttrs->lcdModePin.Port,hwAttrs->lcdModePin.pin,GPIO_PIN_SET);
  /* Set LCD CSn low (spi active) */
  HAL_GPIO_WritePin(hwAttrs->lcdCsnPin.Port,hwAttrs->lcdCsnPin.pin,GPIO_PIN_RESET); 
  
  LCD_sendArray(handle, (char const*)object->lcdBuffers[0].pcBuffer, numPoint *2);
 
  HAL_GPIO_WritePin(hwAttrs->lcdCsnPin.Port,hwAttrs->lcdCsnPin.pin,GPIO_PIN_SET);
}
/*
 *  ======== LCD_doBufferSetHLine ========
 *  This function draws a horizontal line into the specified buffer.
 *  If blocking is set to true, the task execution will be blocked until all
 *  buffer modifications have finished.
 */
static void LCD_doBufferSetHLine(LCD_Handle handle, unsigned char ucXFrom,
                                 unsigned char ucXTo, unsigned char ucY, bool blocking)
{
  
  /* Get pointer to the LCD object */
  LCD_Object *object = handle->object;
  
 
  /* Switch draw direction if ucXTo < ucXFrom */
  if (ucXTo < ucXFrom)
  {
    unsigned char ucTemp = ucXFrom;
    ucXFrom = ucXTo;
    ucXTo = ucTemp;
  }
  
  /* Draw line */
  LCD_fullRegion(handle,object->pColorInfo->frontColor,ucXFrom,ucXTo,ucY,ucY);
  
}

/*
 *  ======== LCD_doBufferClearHLine ========
 *  This function clears a horizontal line from the specified buffer.
 *  If blocking is set to true, the task execution will be blocked until all
 *  buffer modifications have finished.
 *
 */
static void LCD_doBufferClearHLine(LCD_Handle handle,
                                   unsigned char ucXFrom, unsigned char ucXTo, unsigned char ucY, bool blocking)
{
  /* Get pointer to the LCD object */
  LCD_Object *object = handle->object;
  
  /* Switch draw direction if ucXTo < ucXFrom */
  if (ucXTo < ucXFrom)
  {
    unsigned char ucTemp = ucXFrom;
    ucXFrom = ucXTo;
    ucXTo = ucTemp;
  }
  
  /* Clear line */
  LCD_fullRegion(handle,object->pColorInfo->blackColor,ucXFrom,ucXTo,ucY,ucY);
  
}
//******************************************************************************
// fn : LCD_doBufferSetVLine
//
// brief : This function draws a vertical line into the specified buffer.If 
//         blocking is set to true, the task execution will be blocked until all
//         buffer modifications have finished.
// 
// param :  handle   -> the lcd handle
//          ucX      -> the  x point
//          ucYFrom  -> the start y point
//          ucYTo    -> the end  y point
//          blocking -> block or not
//
// return : none
static void LCD_doBufferSetVLine(LCD_Handle handle,  unsigned char ucX,
                                 unsigned char ucYFrom, unsigned char ucYTo, bool blocking)
{
  
  /* Get pointer to the LCD object */
  LCD_Object *object = handle->object;
  

  /* Draw line */
  LCD_fullRegion(handle,object->pColorInfo->frontColor,ucX,ucX,ucYFrom,ucYTo);
  
  

}
//******************************************************************************
// fn : LCD_doBufferClearVLine
//
// brief : This function sets a pixel in the specified buffer.
//
// param : handle -> the lcd handle
//         color  -> true = fill with frontColor
//                   false = fill with blackColor
//         ucX    -> the x position(0-127)
//         ucY    -> the y position(0-127)
//
// return : none
static void LCD_doBufferClearVLine(LCD_Handle handle,  unsigned char ucX,
                                   unsigned char ucYFrom, unsigned char ucYTo, bool blocking)
{
  /* Get pointer to the LCD object */
  LCD_Object *object = handle->object;

  /* Clear line from buffer */
  LCD_fullRegion(handle,object->pColorInfo->blackColor,ucX,ucX,ucYFrom,ucYTo);

}

//******************************************************************************
// fn : LCD_doBufferSetPx
//
// brief : This function sets a pixel in the specified buffer.
//
// param : handle -> the lcd handle
//         color  -> true = fill with frontColor
//                   false = fill with blackColor
//         ucX    -> the x position(0-127)
//         ucY    -> the y position(0-127)
//
// return : none
static void LCD_doBufferSetPx(LCD_Handle handle,unsigned char color, unsigned char ucX,
                              unsigned char ucY)
{
  unsigned short rgbData;

  /* Get pointer to the LCD object */
  LCD_Object *object = handle->object;
  
  if(color)
  {
    rgbData = object->pColorInfo->frontColor;
  }
  else
  {
    rgbData = object->pColorInfo->blackColor;
  }

  /* Draw pixel */
  LCD_setRegion(handle,ucX,ucY,ucX+1,ucY+1);
  
  LCD_sendData(handle, (char const*)&rgbData,2);
   
}
