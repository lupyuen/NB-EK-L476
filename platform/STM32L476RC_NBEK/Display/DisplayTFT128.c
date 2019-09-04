/*
 *             Copyright (c) 2017, Ghostyu Co.,Ltd.
 *                      All rights reserved.
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
/* -----------------------------------------------------------------------------
 *  Includes
 * -----------------------------------------------------------------------------
 */
//
#include <stdio.h>

#include "stm32l4xx_hal.h"
#include "stm32l4xx_nbek.h"

#include "Display.h"
#include "DisplayTFT128.h"
#include "TFT128.h"


#include "stm32l4xx_hal_gpio.h"
#include "stm32l4xx_hal_spi.h"
/* -----------------------------------------------------------------------------
 *  Constants and macros
 * -----------------------------------------------------------------------------
 */
// Timeout of semaphore that controls exclusive to the LCD (500 ms)
#define ACCESS_TIMEOUT    50000

/* -----------------------------------------------------------------------------
 *   Type definitions
 * -----------------------------------------------------------------------------
 */


/* -----------------------------------------------------------------------------
 *                           Local variables
 * -----------------------------------------------------------------------------
 */
/* Display function table for tft1.44 128x128 implementation */
const Display_FxnTable DisplayTFT128_fxnTable = {
    DisplayTFT128_open,
    DisplayTFT128_clear,
    DisplayTFT128_clearLines,
    DisplayTFT128_put5,
    DisplayTFT128_close,
    DisplayTFT128_control,
    DisplayTFT128_getType,
};

/* -----------------------------------------------------------------------------
 *                                          Functions
 * -----------------------------------------------------------------------------
 */
//******************************************************************************
// fn :         DisplayTFT128_open
//
// brief :      Initialize the LCD
//
// descr :     Initializes the pins used by the LCD, creates resource access
//              protection semaphore, turns on the LCD device, initializes the
//              frame buffer, initializes to white background/dark foreground,
//              and finally clears the object->displayColor.
//
// param :     hDisplay - pointer to Display_Config struct
//             params - display parameters
//
// return :    Pointer to Display_Config struct
Display_Handle DisplayTFT128_open(Display_Handle hDisplay,
                                    Display_Params *params)
{
    DisplayTFT128_HWAttrs *hwAttrs = (DisplayTFT128_HWAttrs *)hDisplay->hwAttrs;
    DisplayTFT128_Object  *object  = (DisplayTFT128_Object  *)hDisplay->object;
        
    object->hPins = &hwAttrs->powerPin;
    HAL_GPIO_WritePin(hwAttrs->powerPin.Port,hwAttrs->powerPin.pin,GPIO_PIN_SET);


    object->lineClearMode = params->lineClearMode;
    object->lcdColor.blackColor = YELLOW;
    object->lcdColor.frontColor = BULE;

    object->lcdBuffers[0].pcBuffer = object->lcdBuffer0;
    object->lcdBuffers[0].bufSize  = 512;  
    
    object->hLcd = LCDTFT_open(&object->lcdBuffers[0], 1, NULL);
    object->hLcd->object->pColorInfo = &object->lcdColor;
    
    if (object->hLcd)
    {
        Color color;
        LCDTFT_bufferClear(object->hLcd);
        LCDTFT_bufferStartCover(object->hLcd); 
        color = BLACK;
        DisplayTFT128_control(hDisplay,CMD_BLACK_COLOR,&color);
        color = WHITE;
        DisplayTFT128_control(hDisplay,CMD_FRONT_COLOR,&color);
        LCDTFT_bufferClear(object->hLcd);
        return hDisplay;
    }
    else
    {
        return NULL;
    }
}
//******************************************************************************
// fn :         DisplayTFT128_clear
//
// brief :      Clears the display
//
// param :       hDisplay - pointer to Display_Config struct
//
// return  :    void
void DisplayTFT128_clear(Display_Handle hDisplay)
{
    DisplayTFT128_Object *object = (DisplayTFT128_Object  *)hDisplay->object;

    if (object->hLcd)
    {
        LCDTFT_bufferClear(object->hLcd);
    }
}
//******************************************************************************
// fn :         DisplayTFT128_clearLines
//
// brief :      Clears lines lineFrom-lineTo of the display, inclusive
//
// param :      hDisplay - pointer to Display_Config struct
//              lineFrom - line index (0 .. )
//              lineTo - line index (0 .. )
//
// return :    void
void DisplayTFT128_clearLines(Display_Handle hDisplay,
                                uint8_t lineFrom, uint8_t lineTo)
{
    DisplayTFT128_Object *object = (DisplayTFT128_Object  *)hDisplay->object;

    if (lineTo < lineFrom)
    {
        lineTo = lineFrom;
    }

    if (object->hLcd)
    {
        uint8_t xMin = 0;
        uint8_t xMax = 127;

        LCDTFT_bufferClearPart(object->hLcd, xMin, xMax,
                            (LCD_Page)lineFrom, (LCD_Page)lineTo);
    }
}


//******************************************************************************
// fn :         DisplayTFT128_put5
//
// brief :     Write a text string to a specific line/column of the display
//
// param :     hDisplay - pointer to Display_Config struct
//             cle - clear the remain space
//             line - line index (0..)
//             column - column index (0..)
//             fmt - format string
//             aN - optional format arguments
//
// return :    void
void DisplayTFT128_put5(Display_Handle hDisplay,uint8_t cle, uint8_t line,
                          uint8_t column, char* fmt, va_list va)
{
    DisplayTFT128_Object *object = (DisplayTFT128_Object  *)hDisplay->object;

    char  dispStr[16] = { 0 };
    uint8_t len = 0;
    uint8_t xp;

    xp          = column * 8;   //ascii =  16 * 8

    len = vsnprintf(dispStr, sizeof(dispStr), fmt, va);
    if(cle)
    {
      for(;len < sizeof(dispStr); len++)
      {
        dispStr[len] = ' ';
      }
    }
    LCDTFT_bufferPrintString(object->hLcd, dispStr, xp, (LCD_Page)line);
}


//******************************************************************************
// fn :         DisplayTFT128_close
//
// brief:       Turns of the display and releases the LCD control pins
//
// param :      hDisplay - pointer to Display_Config struct
//
// return :     void
void DisplayTFT128_close(Display_Handle hDisplay)
{
    DisplayTFT128_Object *object = (DisplayTFT128_Object  *)hDisplay->object;

    if (object->hPins == NULL)
    {
        return;
    }

    // Turn off the display
    object->hPins = NULL;

    LCDTFT_close(object->hLcd);
    object->hLcd = NULL;
}

//******************************************************************************
// fn :      DisplayTFT128_control
//
// brief :   Function for setting control parameters of the Display driver
//           after it has been opened.
//
// param :   hDisplay - pointer to Display_Config struct
//           cmd - command to execute
//           arg - argument to the command
//
// return : DISPLAY_STATUS_UNDEFINEDCMD because no commands are supported
int DisplayTFT128_control(Display_Handle handle, unsigned int cmd, void *arg)
{
  DisplayTFT128_Object *object = (DisplayTFT128_Object  *)handle->object;
  
  if(CMD_BLACK_COLOR == cmd)
  {
    object->lcdColor.blackColor = (Color)(*((int*)arg));
  }
  else if(CMD_FRONT_COLOR == cmd)
  {
    object->lcdColor.frontColor = (Color)(*((int*)arg));
  }
  return DISPLAY_STATUS_UNDEFINEDCMD;
}

//******************************************************************************
// fn :         DisplayTFT128_getType
//
// brief :      Returns type of transport
//
// param :
//
// return :     Display type define LCD
unsigned int DisplayTFT128_getType(void)
{
    return Display_Type_LCD;
}
