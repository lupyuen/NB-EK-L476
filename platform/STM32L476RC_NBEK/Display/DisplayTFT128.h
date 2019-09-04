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
#ifndef _DISPLAY_TFT144_128_128_H_
#define _DISPLAY_TFT144_128_128_H_

#include <stdio.h>
#include <stdint.h>

#include "stm32l4xx_hal_spi.h"
#include "Display.h"
#include "TFT128.h"

//include the liteos header files
#include "los_mux.h"


/*
 * 相关的操作函数
 */
extern const Display_FxnTable DisplayTFT128_fxnTable;

/*
 * lcd 
 */
typedef struct DisplayTFT128_HWAttrs
{
    LCD_Handle   lcdHandle;
    PIN_HWAttrs  powerPin;
    
} DisplayTFT128_HWAttrs;


typedef struct DisplayTFT128_Object
{
    PIN_HWAttrs*          hPins;
    LCD_Handle            hLcd;
    Display_LineClearMode lineClearMode;
    uint32_t              lcdMutex;
    LCD_Color             lcdColor;
    uint16_t              lcdBuffer0[512];
    LCD_Buffer            lcdBuffers[1];
} DisplayTFT128_Object, *DisplayTFT128_Handle;

typedef enum
{
  CMD_BLACK_COLOR,
  CMD_FRONT_COLOR
}LCD_CTL_CMD;

/*
 * 
 */
Display_Handle DisplayTFT128_open(Display_Handle, Display_Params * params);
/*
 *
 */
void           DisplayTFT128_clear(Display_Handle handle);
/*
 *
 */
void           DisplayTFT128_clearLines(Display_Handle handle,
                                          uint8_t fromLine,
                                          uint8_t toLine);
/*
 *
 */
void           DisplayTFT128_put5(Display_Handle handle,uint8_t cle, uint8_t line,
                                    uint8_t column, char* fmt,
                                    va_list va);
/*
 *
 */
void DisplayTFT128_close(Display_Handle);
/*
 *
 */
int DisplayTFT128_control(Display_Handle handle, unsigned int cmd, void *arg);
/*
 *
 */
unsigned int   DisplayTFT128_getType(void);

#endif      //_DISPLAY_TFT144_128_128_H_
